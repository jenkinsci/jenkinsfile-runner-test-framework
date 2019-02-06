#!/bin/bash
set -e

# Path to this source file, stored in BASH_SOURCE
root_directory=$(dirname "${BASH_SOURCE[0]}")
sources="$root_directory/src"
sh_unit_directory="$root_directory/.shunit2"
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
# shellcheck source=src/hooks/logs.inc
. "$sources"/hooks/logs.inc
# shellcheck source=src/hooks/result.inc
. "$sources"/hooks/result.inc
# shellcheck source=src/hooks/workspace.inc
. "$sources"/hooks/workspace.inc
# shellcheck source=src/cwp/custom-war-packager.inc
. "$sources"/cwp/custom-war-packager.inc
# shellcheck source=src/jfr/jenkinsfile-runner.inc
. "$sources"/jfr/jenkinsfile-runner.inc

init_framework() {
    . $sh_unit_directory/shunit2
}
