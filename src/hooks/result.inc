#!/bin/bash
set -e

# Check the execution of some command is ok
# $1: Returned code by process
execution_success() {

    if [ "$#" -eq 1 ]
    then
        if [ -z "${_shunit_test_}" ]
        then
            test_name="Execution"
        else
            test_name="Execution of test '${_shunit_test_}'"
        fi

        assertEquals "$test_name should retrieve exit code 0" "0" "$1"
        return 0
    else
        echo "Error. Missing parameters:"
        echo "   Returned code by process"
        return 1
    fi

}

# Check the execution message of some command contains an expected pattern
# $1: Returned message
# $2: Expected contained value
message_contains() {

    if [ "$#" -eq 2 ]
    then
        if [ -z "${_shunit_test_}" ]
        then
            test_name="Message from execution"
        else
            test_name="Message from execution of test '${_shunit_test_}'"
        fi

        assertContains "$test_name should contain '$2'" "$1" "$2"
        return 0
    else
        echo "Error. Missing parameters:"
        echo "   Returned message"
        echo "   Expected contained value"
        return 1
    fi

}

# Check the execution message of some command contains an expected pattern
# $1: Returned message
# $2: Expected contained value
message_not_contains() {

    if [ "$#" -eq 2 ]
    then
        if [ -z "${_shunit_test_}" ]
        then
            test_name="Message from execution"
        else
            test_name="Message from execution of test '${_shunit_test_}'"
        fi

        assertNotContains "$test_name should not contain '$2'" "$1" "$2"
        return 0
    else
        echo "Error. Missing parameters:"
        echo "   Returned message"
        echo "   Expected contained value"
        return 1
    fi

}

# Check a generic execution is ok
# $1: Returned code by the process
# $2: Execution log
# $3: Expected excerpt
execution_should_success() {

    if [ "$#" -eq 3 ]
    then
        execution_success "$1"
        message_contains "$2" "$3"
    else
        echo "Error. Missing parameters:"
        echo "   Returned code by the process"
        echo "   Execution log"
        echo "   Expected message"
        return 1
    fi

}

# Check the execution of the Jenkinsfile should succed
# $1: Returned code by the process
# $2: Execution log
jenkinsfile_execution_should_succed() {

    if [ "$#" -eq 2 ]
    then
        execution_success "$1"
        message_contains "$2" "[Pipeline] End of Pipeline"
        message_contains "$2" "Finished: SUCCESS"
    else
        echo "Error. Missing parameters:"
        echo "   Returned code by the process"
        echo "   Execution log"
        return 1
    fi

}

# Check the execution of the Jenkinsfile should fail
# $1: Returned code by the process
# $2: Execution log
jenkinsfile_execution_should_fail() {

    if [ "$#" -eq 2 ]
    then
        # The execution should be success while the execution of the pipeline should be faild
        execution_success "$1"
        message_contains "$2" "[Pipeline] End of Pipeline"
        message_contains "$2" "Finished: FAILURE"
    else
        echo "Error. Missing parameters:"
        echo "   Returned code by the process"
        echo "   Execution log"
        return 1
    fi

}

# Check the execution of the Jenkinsfile should be unstable
# $1: Returned code by the process
# $2: Execution log
jenkinsfile_execution_should_be_unstable() {

    if [ "$#" -eq 2 ]
    then
        # The execution should be success while the execution of the pipeline should fail
        execution_success "$1"
        message_contains "$2" "[Pipeline] End of Pipeline"
        message_contains "$2" "Finished: UNSTABLE"
    else
        echo "Error. Missing parameters:"
        echo "   Returned code by the process"
        echo "   Execution log"
        return 1
    fi

}

# Check the generation of the Dockerfile should fail
# $1: Returned code by the process
# $2: Execution log
docker_generation_should_fail() {

    if [ "$#" -eq 2 ]
    then
        # The execution should be success while the generation should have error messages in the log
        execution_success "$1"
        message_contains "$2" "[ERROR]"
    else
        echo "Error. Missing parameters:"
        echo "   Returned code by the process"
        echo "   Execution log"
        return 1
    fi

}
