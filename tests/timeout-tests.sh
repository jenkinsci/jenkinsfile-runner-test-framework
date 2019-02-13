#!/bin/bash
set -e

current_directory=$(pwd)
test_framework_directory="$current_directory/.."
working_directory="$current_directory/.testing"

version="256.0-test"
jenkinsfile_runner_tag="jenkins-experimental/jenkinsfile-runner-test-image"

. $test_framework_directory/init-jfr-test-framework.inc

setUp() {
  downloaded_cwp_jar=$(download_cwp "$working_directory")
  execute_cwp_jar_and_generate_docker_image "$working_directory" "$downloaded_cwp_jar" "$version" "$current_directory/test_resources/test_with_tag/packager-config.yml" "$jenkinsfile_runner_tag"  | grep 'Successfully tagged'
}

test_timeout() {
  run_jfr_docker_image "$jenkinsfile_runner_tag" "$current_directory/test_resources/test_timeout/Jenkinsfile"
  jenkinsfile_execution_should_succed "$?"
}

# Prepare docker image
setUp

# Set timeout value to 10 seconds
set_timeout 10

# Eval the timeout
set +e
result=$(eval run_with_timeout "test_timeout" | grep "[ERROR]*timeout")
set -e

# Print result
if [[ "$result" != *"[ERROR] test_timeout timeout. Passed 10 seconds"* ]]
then
  echo "test FAIL: test_timeout should timeout"
  echo "1 test executed"
else
  echo "test OK"
  echo "1 test executed"
fi
