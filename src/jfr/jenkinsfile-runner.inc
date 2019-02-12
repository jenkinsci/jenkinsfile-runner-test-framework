#!/bin/bash
set -e

# function to run a docker image
# $1: tag of the docker image
# $2: Path to Jenkinsfile
# $3: (Optional) Docker options
# $4: (Optional) Jenksinfile-runner options
run_jfr_docker_image() {

    if [ "$#" -gt 1 ]
    then
        if [ -z "$JAVA_OPTS" ]
        then
            if [ ! -z "${_shunit_test_}" ]
            then
                docker run --rm $3 -v "$2":/workspace/Jenkinsfile "$1" $4 2>&1 | tee "${_shunit_test_}.log"
            else
                docker run --rm $3 -v "$2":/workspace/Jenkinsfile "$1 $4"
            fi
        else
            if [ ! -z "${_shunit_test_}" ]
            then
                docker run -e JAVA_OPTS="$JAVA_OPTS" --rm $3 -v "$2":/workspace/Jenkinsfile "$1" $4 2>&1 | tee "${_shunit_test_}.log"
            else
                docker run -e JAVA_OPTS="$JAVA_OPTS" --rm $3 -v "$2":/workspace/Jenkinsfile "$1 $4"
            fi
        fi

        return 0
    else
        echo "Error. Missing parameters:"
        echo "   Tag for the docker image to run"
        echo "   Path to Jenkinsfile"
        echo "   (Optional) Docker options"
        return 1
    fi

}