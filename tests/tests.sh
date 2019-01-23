#!/bin/bash
set -e

current_directory=$(pwd)
test_framework_directory="$current_directory/.."
sh_unit_directory="$current_directory/.testing/shunit2"

version="256.0-test"
CWP_version="1.5"
jenkinsfile_runner_tag="jenkins-experimental/jenkinsfile-runner-test-image"

downloaded_cwp_jar="to_update"

. $test_framework_directory/utilities/utils.inc
. $test_framework_directory/utilities/cwp/custom-war-packager.inc
. $test_framework_directory/utilities/jfr/jenkinsfile-runner.inc

oneTimeSetUp() {
  downloaded_cwp_jar=$(download_cwp "$test_framework_directory" "$CWP_version")
}

setUp() {
  echo "Initializing the test case. Ignoring so far"
}

test_with_tag() {
  jfr_tag=$(execute_cwp_jar_and_generate_docker_image "$test_framework_directory" "$downloaded_cwp_jar" "$version" "$current_directory/test_resources/test_with_tag/packager-config.yml" "$jenkinsfile_runner_tag" | grep 'Successfully tagged')
  assertEquals "Should retrieve exit code 0" "0" "$?"
  assertContains "Should contain the given tag" "$jfr_tag" "$jenkinsfile_runner_tag"
  
  result=$(run_jfr_docker_image "$jenkinsfile_runner_tag" "$current_directory/test_resources/test_with_tag/Jenkinsfile")
  assertEquals "Should retrieve exit code 0" "0" "$?"
  assertContains "Should execute the Jenkinsfile successfully" "$result" "[Pipeline] End of Pipeline"
  assertContains "Should execute the Jenkinsfile successfully" "$result" "Finished: SUCCESS"
}

test_with_default_tag() {
  jfr_tag=$(execute_cwp_jar_and_generate_docker_image "$test_framework_directory" "$downloaded_cwp_jar" "$version" "$current_directory/test_resources/test_with_tag/packager-config.yml" | grep 'Successfully tagged')
  assertEquals "Should retrieve exit code 0" "0" "$?"
  assertNotContains "Should not contain the given tag" "$jfr_tag" "$jenkinsfile_runner_tag"
}

oneTimeTearDown() {
  echo "Cleaning the test suite. Ignoring so far"
}

tearDown() {
  echo "Cleaning the test case. Ignoring so far"
}

. $sh_unit_directory/shunit2
