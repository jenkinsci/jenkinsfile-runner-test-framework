#!/bin/bash
set -e

current_directory=$(pwd)
test_framework_directory="$current_directory/.."
working_directory="$current_directory/.testing"

version="256.0-test"
jenkinsfile_runner_tag="jenkins-experimental/jenkinsfile-runner-test-image"

downloaded_cwp_jar="to_update"

. $test_framework_directory/init-jfr-test-framework.inc

oneTimeSetUp() {
  downloaded_cwp_jar=$(download_cwp "$working_directory")
}

test_with_tag() {
  jfr_tag=$(execute_cwp_jar_and_generate_docker_image "$working_directory" "$downloaded_cwp_jar" "$version" "$current_directory/test_resources/test_with_tag/packager-config.yml" "$jenkinsfile_runner_tag" | grep 'Successfully tagged')
  execution_should_success "$?" "$jenkinsfile_runner_tag" "$jfr_tag"

  run_jfr_docker_image "$jenkinsfile_runner_tag" "$current_directory/test_resources/test_with_tag/Jenkinsfile"
  jenkinsfile_execution_should_succed "$?"
}

test_java_opts() {
  jfr_tag=$(execute_cwp_jar_and_generate_docker_image "$working_directory" "$downloaded_cwp_jar" "$version" "$current_directory/test_resources/test_with_tag/packager-config.yml" "$jenkinsfile_runner_tag" | grep 'Successfully tagged')
  execution_should_success "$?" "$jenkinsfile_runner_tag" "$jfr_tag"

  run_jfr_docker_image "$jenkinsfile_runner_tag" "$current_directory/test_resources/test_with_tag/Jenkinsfile"
  jenkinsfile_execution_should_succed "$?"

  export JAVA_OPTS="-Xmx1M -Xms100G"
  run_jfr_docker_image "$jenkinsfile_runner_tag" "$current_directory/test_resources/test_with_tag/Jenkinsfile"
  assertEquals "Should retrieve exit code 0" "0" "$?"
  logs_not_contains "[Pipeline] End of Pipeline"
  logs_not_contains "Finished: SUCCESS"
  logs_contains "Initial heap size set to a larger value than the maximum heap size"
  unset JAVA_OPTS
}

test_with_default_tag() {
  jfr_tag=$(execute_cwp_jar_and_generate_docker_image "$working_directory" "$downloaded_cwp_jar" "$version" "$current_directory/test_resources/test_with_tag/packager-config.yml" | grep 'Successfully tagged')
  execution_should_success "$?" "test_with_default_tag" "$jfr_tag"
}

test_download_cwp_version() {
  default_cwp_jar=$(download_cwp "$working_directory")
  execution_should_success "$?" "cwp-cli-$DEFAULT_CWP_VERSION.jar" "$default_cwp_jar"

  another_cwp_jar=$(download_cwp "$working_directory" "1.3")
  execution_should_success "$?" "cwp-cli-1.3.jar" "$another_cwp_jar"
}

test_with_tag_using_cwp_docker_image() {
  jfr_tag=$(generate_docker_image_from_cwp_docker_image "$current_directory/test_resources/test_with_tag_using_cwp_docker_image/packager-config.yml" "$jenkinsfile_runner_tag" | grep 'Successfully tagged')
  execution_should_success "$?" "$jenkinsfile_runner_tag" "$jfr_tag"

  run_jfr_docker_image "$jenkinsfile_runner_tag" "$current_directory/test_resources/test_with_tag_using_cwp_docker_image/Jenkinsfile"
  jenkinsfile_execution_should_succed "$?"
}

test_with_default_tag_using_cwp_docker_image() {
  jfr_tag=$(generate_docker_image_from_cwp_docker_image "$current_directory/test_resources/test_with_default_tag_using_cwp_docker_image/packager-config.yml" | grep 'Successfully tagged')
  execution_should_success "$?" "test_with_default_tag_using_cwp_docker_image" "$jfr_tag"
}

test_failing_docker_image() {
  result=$(execute_cwp_jar_and_generate_docker_image "$working_directory" "$downloaded_cwp_jar" "$version" "$current_directory/test_resources/test_failing_docker_image/packager-config.yml")
  docker_generation_should_fail "$?" "$result"
}

test_jenkinsfile_fail() {
  jfr_tag=$(execute_cwp_jar_and_generate_docker_image "$working_directory" "$downloaded_cwp_jar" "$version" "$current_directory/test_resources/test_with_tag/packager-config.yml" "$jenkinsfile_runner_tag" | grep 'Successfully tagged')
  execution_should_success "$?" "$jenkinsfile_runner_tag" "$jfr_tag"

  run_jfr_docker_image "$jenkinsfile_runner_tag" "$current_directory/test_resources/test_jenkinsfile_fail/Jenkinsfile"
  jenkinsfile_execution_should_fail "$?"
}

test_jenkinsfile_unstable() {
  jfr_tag=$(execute_cwp_jar_and_generate_docker_image "$working_directory" "$downloaded_cwp_jar" "$version" "$current_directory/test_resources/test_with_tag/packager-config.yml" "$jenkinsfile_runner_tag" | grep 'Successfully tagged')
  execution_should_success "$?" "$jenkinsfile_runner_tag" "$jfr_tag"

  run_jfr_docker_image "$jenkinsfile_runner_tag" "$current_directory/test_resources/test_jenkinsfile_unstable/Jenkinsfile"
  jenkinsfile_execution_should_be_unstable "$?"
}

init_framework