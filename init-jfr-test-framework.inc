#!/bin/bash
set -e

sources=$(dirname "${BASH_SOURCE[0]}")
export DEFAULT_CWP_VERSION="1.5"

. "$sources"/utilities/utils.inc
. "$sources"/utilities/cwp/custom-war-packager.inc
. "$sources"/utilities/jfr/jenkinsfile-runner.inc
