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

#
# Use a JCasC configuration to build the JFR image with a tool, in this case a maven instance.  The
# current solution is a bit of a hack, as I could not determine how to get CasC to download a maven
# instance, so I am doing that in the Jenkinsfile.
#
# FIXME for this and other tests we are copying casc.yml to the current directory so that it is
# referenced properly in packager-config-yml.  It would be better to put an environment variable
# or placeholder in packager-config.yml and update that with the real location before running.
#
test_cwp_jcasc_with_tool() {
  cp $current_directory/test_resources/test_cwp_casc_with_tool/casc.yml .
  jfr_tag=$(execute_cwp_jar_and_generate_docker_image "$working_directory" "$downloaded_cwp_jar" "$version" "$current_directory/test_resources/test_cwp_casc_with_tool/packager-config.yml" "$jenkinsfile_runner_tag" | grep 'Successfully tagged')
  execution_should_success "$?" "$jfr_tag" "$jenkinsfile_runner_tag"

  result=$(run_jfr_docker_image "$jenkinsfile_runner_tag" "$current_directory/test_resources/test_cwp_casc_with_tool/Jenkinsfile")
  jenkinsfile_execution_should_succed "$?" "$result"
  assertContains "Should contain the correct maven version" "$result" "Apache Maven 3.3.3"
}

#
# Use a JCasC configuration to build the JFR image with a global library.  Then try to
# reference methods in that library
#
# TODO we may want to create our own library for this and not depend on code from other projects.
#
test_jcasc_with_pipeline_library() {
  cp $current_directory/test_resources/test_cwp_casc_with_pipeline_library/casc.yml .
  jfr_tag=$(execute_cwp_jar_and_generate_docker_image "$working_directory" "$downloaded_cwp_jar" "$version" "$current_directory/test_resources/test_cwp_casc_with_pipeline_library/packager-config.yml" "$jenkinsfile_runner_tag" | grep 'Successfully tagged')
  execution_should_success "$?" "$jfr_tag" "$jenkinsfile_runner_tag"

  result=$(run_jfr_docker_image "$jenkinsfile_runner_tag" "$current_directory/test_resources/test_cwp_casc_with_pipeline_library/Jenkinsfile")
  jenkinsfile_execution_should_succed "$?" "$result"
  assertContains "Should contain calls to awesome-lib" "$result" "On Jenkins Infra? false"
}

init_framework
