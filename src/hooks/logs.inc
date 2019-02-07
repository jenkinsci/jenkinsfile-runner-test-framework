#!/bin/bash
set -e

# Check if the execution log of some command contains an expected pattern
# $1: Expected contained value
# $2: (Optional) Logs from the test or returned message
logs_contains() {

    if [ "$#" -ge 1 ]
    then
        test_name=$(get_test_name_message)

        if [ "$#" -eq 2 ]
        then
            assertContains "$test_name should contain '$1'" "$2" "$1"
            return 0
        elif [ -e "${_shunit_test_}.log" ]
        then
            text_contained=0
            if [[ "$(cat ${_shunit_test_}.log)" == *"$1"* ]]; then
                text_contained=1
            fi

            assertEquals "$test_name should contain '$1'" "1" "$text_contained"
            return 0
        else
            echo "Error. No log to inspect"
            return 1
        fi
    else
        echo "Error. Missing parameters:"
        echo "   Expected contained value"
        echo "   (Optional) Logs from the test or returned message"
        return 1
    fi

}

# Check if the execution log of some command does not contain an expected pattern
# $1: Expected contained value
# $2: (Optional) Logs from the test or returned message
logs_not_contains() {

    if [ "$#" -ge 1 ]
    then
        test_name=$(get_test_name_message)

        if [ "$#" -eq 2 ]
        then
            assertNotContains "$test_name should contain '$1'" "$2" "$1"
            return 0
        elif [ -e "${_shunit_test_}.log" ]
        then
            text_contained=0
            if [[ "$(cat ${_shunit_test_}.log)" == *"$1"* ]]; then
                text_contained=1
            fi

            assertEquals "$test_name should not contain '$1'" "0" "$text_contained"
            return 0
        else
            echo "Error. No log to inspect"
            return 1
        fi
    else
        echo "Error. Missing parameters:"
        echo "   Expected contained value"
        echo "   (Optional) Logs from the test or returned message"
        return 1
    fi

}

# Get proper message to display when referring to the execution
get_test_name_message() {
    if [ -z "${_shunit_test_}" ]
    then
        test_name="Message from execution"
    else
        test_name="Message from execution of test '${_shunit_test_}'"
    fi

    echo "$test_name"
}
