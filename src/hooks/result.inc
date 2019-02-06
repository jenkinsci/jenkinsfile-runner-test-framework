#!/bin/bash
set -e

# Path to this source file, stored in BASH_SOURCE
root_directory=$(dirname "${BASH_SOURCE[0]}")
# shellcheck source=src/utilities/utils.inc
. "$root_directory"/logs.inc

# Check the execution of some command is ok
# $1: Returned code by process
execution_success() {

    if [ "$#" -eq 1 ]
    then
        # shellcheck disable=SC2154
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

# Check a generic execution is ok
# $1: Returned code by the process
# $2: Expected excerpt
# $2: (Optional) Execution log
execution_should_success() {

    if [ "$#" -eq 3 ]
    then
        execution_success "$1"
        logs_contains "$2" "$3"
    elif [ "$#" -eq 3 ]
    then
        execution_success "$1"
        logs_contains "$2"
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
# $2: (Optional) Execution log
jenkinsfile_execution_should_succed() {

    if [ "$#" -eq 2 ]
    then
        execution_success "$1"
        logs_contains "[Pipeline] End of Pipeline" "$2"
        logs_contains "Finished: SUCCESS" "$2"
    elif [ "$#" -eq 1 ]
        then
            execution_success "$1"
            logs_contains "[Pipeline] End of Pipeline"
            logs_contains "Finished: SUCCESS"
    else
        echo "Error. Missing parameters:"
        echo "   Returned code by the process"
        echo "   Execution log"
        return 1
    fi

}

# Check the execution of the Jenkinsfile should fail
# $1: Returned code by the process
# $2: (Optional) Execution log
jenkinsfile_execution_should_fail() {

    if [ "$#" -eq 2 ]
    then
        # The execution should be success while the execution of the pipeline should fail
        execution_success "$1"
        logs_contains "[Pipeline] End of Pipeline" "$2"
        logs_contains "Finished: FAILURE" "$2"
    elif [ "$#" -eq 1 ]
    then
        # The execution should be success while the execution of the pipeline should fail
        execution_success "$1"
        logs_contains "[Pipeline] End of Pipeline"
        logs_contains "Finished: FAILURE"
    else
        echo "Error. Missing parameters:"
        echo "   Returned code by the process"
        echo "   Execution log"
        return 1
    fi

}

# Check the execution of the Jenkinsfile should be unstable
# $1: Returned code by the process
# $2: (Optional) Execution log
jenkinsfile_execution_should_be_unstable() {

    if [ "$#" -eq 2 ]
    then
        # The execution should be success while the execution of the pipeline should be unstable
        execution_success "$1"
        logs_contains "[Pipeline] End of Pipeline" "$2"
        logs_contains "Finished: UNSTABLE" "$2"
    elif [ "$#" -eq 1 ]
    then
        # The execution should be success while the execution of the pipeline should be unstable
        execution_success "$1"
        logs_contains "[Pipeline] End of Pipeline"
        logs_contains "Finished: UNSTABLE"
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
        logs_contains "[ERROR]" "$2"
    else
        echo "Error. Missing parameters:"
        echo "   Returned code by the process"
        echo "   Execution log"
        return 1
    fi

}
