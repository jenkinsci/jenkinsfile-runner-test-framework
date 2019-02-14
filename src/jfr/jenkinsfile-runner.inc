#!/bin/bash
set -e

# function to run a docker image
# $1: tag of the docker image
# $2: Path to Jenkinsfile
run_jfr_docker_image() {
    if [ "$#" -eq 2 ]
    then
        run_jfr_docker_image_with_docker_and_jfr_options "$1" "$2" "" ""
        return 0
    else
        echo "Error. Missing parameters:"
        echo "   Tag for the docker image to run"
        echo "   Path to Jenkinsfile"
    fi
}

# function to run a docker image with additional docker options
# $1: tag of the docker image
# $2: Path to Jenkinsfile
# $3: Docker options
run_jfr_docker_image_with_docker_options() {
    if [ "$#" -eq 3 ]
    then
        run_jfr_docker_image_with_docker_and_jfr_options "$1" "$2" "$3" ""
        return 0
    else
        echo "Error. Missing parameters:"
        echo "   Tag for the docker image to run"
        echo "   Path to Jenkinsfile"
        echo "   Docker Options"
    fi
}


# function to run a docker image with additional options passed to Jenkinsfile-runner
# $1: tag of the docker image
# $2: Path to Jenkinsfile
# $3: Jenksinfile-runner options
run_jfr_docker_image_with_jfr_options() {
    if [ "$#" -eq 3 ]
    then
       run_jfr_docker_image_with_docker_and_jfr_options "$1" "$2" "" "$3"
       return 0
    else
        echo "Error. Missing parameters:"
        echo "   Tag for the docker image to run"
        echo "   Path to Jenkinsfile"
        echo "   Jenksinfile-runner Options"
    fi
}


# function to run a docker image with both additional docker options and options for Jenkinsfile-runner
# $1: tag of the docker image
# $2: Path to Jenkinsfile
# $3: Docker options
# $4: Jenksinfile-runner options
run_jfr_docker_image_with_docker_and_jfr_options() {
    if [ "$#" -eq 4 ]
    then
        if [ -z "$JAVA_OPTS" ]
        then
            if [ ! -z "${_shunit_test_}" ]
            then
                docker run --rm $3 -v "$2":/workspace/Jenkinsfile "$1" $4 2>&1 | tee "${_shunit_test_}.log"
            else
                docker run --rm $3 -v "$2":/workspace/Jenkinsfile "$1" $4
            fi
        else
            if [ ! -z "${_shunit_test_}" ]
            then
                docker run -e JAVA_OPTS="$JAVA_OPTS" --rm $3 -v "$2":/workspace/Jenkinsfile "$1" $4 2>&1 | tee "${_shunit_test_}.log"
            else
                docker run -e JAVA_OPTS="$JAVA_OPTS" --rm $3 -v "$2":/workspace/Jenkinsfile "$1" $4
            fi
        fi
    else
        echo "Error. Missing parameters:"
        echo "   Tag for the docker image to run"
        echo "   Path to Jenkinsfile"
        echo "   Docker Options"
        echo "   Jenksinfile-runner Options"
    fi
}

