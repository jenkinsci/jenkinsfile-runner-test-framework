#!/bin/bash
set -e

current_directory=$(pwd)
test_framework_directory="$current_directory/.."
working_directory="$current_directory/.testing"
sh_unit_directory="$working_directory/shunit2"

version="256.0-test"
jenkinsfile_runner_tag="jenkins-experimental/jenkinsfile-runner-test-image"

downloaded_cwp_jar="to_update"

. $test_framework_directory/init-jfr-test-framework.inc

oneTimeSetUp() {
  downloaded_cwp_jar=$(download_cwp "$test_framework_directory")
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

test_java_opts() {
  jfr_tag=$(execute_cwp_jar_and_generate_docker_image "$test_framework_directory" "$downloaded_cwp_jar" "$version" "$current_directory/test_resources/test_with_tag/packager-config.yml" "$jenkinsfile_runner_tag" | grep 'Successfully tagged')
  assertEquals "Should retrieve exit code 0" "0" "$?"
  assertContains "Should contain the given tag" "$jfr_tag" "$jenkinsfile_runner_tag"

  result=$(run_jfr_docker_image "$jenkinsfile_runner_tag" "$current_directory/test_resources/test_with_tag/Jenkinsfile")
  assertEquals "Should retrieve exit code 0" "0" "$?"
  assertContains "Should execute the Jenkinsfile successfully" "$result" "[Pipeline] End of Pipeline"
  assertContains "Should execute the Jenkinsfile successfully" "$result" "Finished: SUCCESS"

  export JAVA_OPTS="-Xmx1M -Xms100G"
  result=$(run_jfr_docker_image "$jenkinsfile_runner_tag" "$current_directory/test_resources/test_with_tag/Jenkinsfile")
  assertEquals "Should retrieve exit code 0" "0" "$?"
  assertNotContains "Should not execute the Jenkinsfile successfully" "$result" "[Pipeline] End of Pipeline"
  assertNotContains "Should not execute the Jenkinsfile successfully" "$result" "Finished: SUCCESS"
  assertContains "Should execute the Jenkinsfile successfully" "$result" "Initial heap size set to a larger value than the maximum heap size"
  unset JAVA_OPTS
}

test_with_default_tag() {
  jfr_tag=$(execute_cwp_jar_and_generate_docker_image "$test_framework_directory" "$downloaded_cwp_jar" "$version" "$current_directory/test_resources/test_with_tag/packager-config.yml" | grep 'Successfully tagged')
  assertEquals "Should retrieve exit code 0" "0" "$?"
  assertNotContains "Should not contain the given tag" "$jfr_tag" "$jenkinsfile_runner_tag"
}

test_download_cwp_version() {
  default_cwp_jar=$(download_cwp "$test_framework_directory")
  assertEquals "Should retrieve exit code 0" "0" "$?"
  assertContains "Should contain the the default version" "$default_cwp_jar" "cwp-cli-$DEFAULT_CWP_VERSION.jar"

  another_cwp_jar=$(download_cwp "$test_framework_directory" "1.3")
  assertEquals "Should retrieve exit code 0" "0" "$?"
  assertContains "Should contain the the given version" "$another_cwp_jar" "cwp-cli-1.3.jar"
}

test_with_tag_using_cwp_docker_image() {
  jfr_tag=$(generate_docker_image_from_cwp_docker_image "$current_directory/test_resources/test_with_tag_using_cwp_docker_image/packager-config.yml" "$jenkinsfile_runner_tag" | grep 'Successfully tagged')
  assertEquals "Should retrieve exit code 0" "0" "$?"
  assertContains "Should contain the given tag" "$jfr_tag" "$jenkinsfile_runner_tag"
  
  result=$(run_jfr_docker_image "$jenkinsfile_runner_tag" "$current_directory/test_resources/test_with_tag_using_cwp_docker_image/Jenkinsfile")
  assertEquals "Should retrieve exit code 0" "0" "$?"
  assertContains "Should execute the Jenkinsfile successfully" "$result" "[Pipeline] End of Pipeline"
  assertContains "Should execute the Jenkinsfile successfully" "$result" "Finished: SUCCESS"
}

test_with_default_tag_using_cwp_docker_image() {
  jfr_tag=$(generate_docker_image_from_cwp_docker_image "$current_directory/test_resources/test_with_default_tag_using_cwp_docker_image/packager-config.yml" | grep 'Successfully tagged')
  assertEquals "Should retrieve exit code 0" "0" "$?"
  assertNotContains "Should not contain the given tag" "$jfr_tag" "$jenkinsfile_runner_tag"
  assertContains "Should contain the name of the test" "$jfr_tag" "test_with_default_tag"
}

. $sh_unit_directory/shunit2
