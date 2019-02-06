#!/bin/bash
set -e

# Path to this source file, stored in BASH_SOURCE
root_directory=$(dirname "${BASH_SOURCE[0]}")
sources="$root_directory/src"
export DEFAULT_CWP_VERSION="1.5"

# shellcheck source=src/utilities/utils.inc
. "$sources"/utilities/utils.inc
# shellcheck source=src/hooks/logs.inc
. "$sources"/hooks/logs.inc
# shellcheck source=src/hooks/result.inc
. "$sources"/hooks/result.inc
# shellcheck source=src/cwp/custom-war-packager.inc
. "$sources"/cwp/custom-war-packager.inc
# shellcheck source=src/jfr/jenkinsfile-runner.inc
. "$sources"/jfr/jenkinsfile-runner.inc
