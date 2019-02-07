#!/bin/bash
set -e

# Path to this source file, stored in BASH_SOURCE
root_directory=$(dirname "${BASH_SOURCE[0]}")
sources="$root_directory/src"
sh_unit_directory="$root_directory/.shunit2"
timeout_test_file="/tmp/with_timeout.sh"
export DEFAULT_CWP_VERSION="1.5"

if [ ! -d "$sh_unit_directory" ]
then
    if [ -z "$1" ] || [ ! -d "$1" ]
    then
        echo "[ERROR] shUnit2 has not been properly initialized"
        exit 1
    else
        sh_unit_directory="$1"
    fi
fi

# shellcheck source=src/utilities/utils.inc
. "$sources"/utilities/utils.inc
# shellcheck source=src/utilities/timeout.inc
. "$sources"/utilities/timeout.inc
# shellcheck source=src/hooks/result.inc
. "$sources"/hooks/result.inc
# shellcheck source=src/hooks/workspace.inc
. "$sources"/hooks/workspace.inc
# shellcheck source=src/cwp/custom-war-packager.inc
. "$sources"/cwp/custom-war-packager.inc
# shellcheck source=src/jfr/jenkinsfile-runner.inc
. "$sources"/jfr/jenkinsfile-runner.inc


search_tests() {

    # Extract the lines with test function names, strip of anything besides the
    # function name, and output everything on a single line.
    test_regex='^\s*((function test[A-Za-z0-9_-]*)|(test[A-Za-z0-9_-]* *\(\)))'
    # shellcheck disable=SC2196
    egrep "${test_regex}" "$0" \
    |sed 's/^[^A-Za-z0-9_-]*//;s/^function //;s/\([A-Za-z0-9_-]*\).*/\1/g' \
    |xargs

}

# Override the suite test to execute the tests with timeout if applies
suite() {

    tests=$(search_tests)
    for name in ${tests}; do
        new_test=$(create_test_with_template "$name" "$timeout_test_file")
        suite_addTest "$new_test"
    done
    # shellcheck disable=SC1090
    . $timeout_test_file
	
}

init_framework() {
    create_test_file "$timeout_test_file"
	# shellcheck disable=SC1090
    . "$sh_unit_directory/shunit2"
}
