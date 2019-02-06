#!/bin/bash
set -e

current_directory=$(pwd)
test_framework_directory="$current_directory/.."
testing_directory="$current_directory/.testing"
working_directory="$testing_directory/workspace-test"
test_file="test.txt"
sh_unit_directory="$testing_directory/shunit2"

. $test_framework_directory/init-jfr-test-framework.inc

oneTimeSetUp() {
  # Removing should not be necessary
  rm -rf "$working_directory"
  mkdir -p "$working_directory"
  echo "This is a fake text in a test file" > "$working_directory/$test_file"
  export WORKSPACE="$working_directory"
}

oneTimeTearDown() {
  rm -rf "$working_directory"
  unset WORKSPACE
}

test_workspace_exists() {
  # Passing another directory to check if exist so we do not use the WORKSPACE environment variable
  workspace_exists "$sh_unit_directory"
}

test_workspace_does_not_exist() {
  workspace_does_not_exist "$working_directory/fake_directory"
}

test_file_exists_in_workspace() {
  file_exists_in_workspace "$test_file"
}

test_file_does_not_exist_in_workspace() {
  file_does_not_exist_in_workspace fake_file.txt
}

test_file_contains_text() {
  file_contains_text "fake text" "$test_file"
}

test_file_does_not_contains_text() {
  file_does_not_contains_text "unexpected text" "$test_file"
}

. $sh_unit_directory/shunit2
