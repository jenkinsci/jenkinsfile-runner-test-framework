#!/bin/bash
set -e

# function to run a docker image
# $1: tag of the docker image
# $2: Path to Jenkinsfile
# $3: Value of JAVA_OPTS
run_jfr_docker_image() {

    if [ "$#" -gt 1 ]
    then
        if [ -z "$JAVA_OPTS" ]
        then
            docker run --rm -v "$2":/workspace/Jenkinsfile "$1"
        else
            docker run -e JAVA_OPTS="$JAVA_OPTS" --rm -v "$2":/workspace/Jenkinsfile "$1"
        fi

        return 0
    else
        echo "Error. Missing parameters:"
        echo "   Tag for the docker image to run"
        echo "   Path to Jenkinsfile"
        return 1
    fi

}