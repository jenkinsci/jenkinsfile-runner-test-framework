#!/bin/bash
set -e

# function to download the CWP jar
# $1: Working directory
# $2: CWP version to use. Optional
download_cwp() {

    if [ "$#" -ge 1 ]
    then
        if [ "$#" -eq 2 ]
        then
            version_to_download="$2"
        else
            version_to_download="$DEFAULT_CWP_VERSION"
        fi

        cwp_jar_file="$1/.cwp-build/cwp-cli-$version_to_download.jar"

        rm -rf "$1/.cwp-build"
        mkdir -p "$1/.cwp-build"

        if [[ $version_to_download == *"SNAPSHOT"* ]]
        then
            wget -O "$cwp_jar_file" https://repo.jenkins-ci.org/snapshots/io/jenkins/tools/custom-war-packager/custom-war-packager-cli/"$version_to_download"/custom-war-packager-cli-"$version_to_download"-jar-with-dependencies.jar
        else
            if [[ $2 == *"rc"* ]]
            then
                wget -O "$cwp_jar_file" https://repo.jenkins-ci.org/incrementals/io/jenkins/tools/custom-war-packager/custom-war-packager-cli/"$version_to_download"/custom-war-packager-cli-"$version_to_download"-jar-with-dependencies.jar
            else
                wget -O "$cwp_jar_file" https://repo.jenkins-ci.org/releases/io/jenkins/tools/custom-war-packager/custom-war-packager-cli/"$version_to_download"/custom-war-packager-cli-"$version_to_download"-jar-with-dependencies.jar
            fi
        fi

        downloaded=$(stat -c%s "$cwp_jar_file")
        if [ "$downloaded" == "0" ]
        then
            echo "CWP jar not found"
            return 1
        else
            echo "$cwp_jar_file"
            return 0
        fi
    else
        echo "Error. Missing parameters:"
        echo "   Working directory"
        echo "   (Optional) CWP version to use. ${DEFAULT_CWP_VERSION} by default"
        return 1
    fi

}

# function to execute a CWP downloaded jar
# $1: Working directory
# $2: Path to cwp.jar
# $3: Jenkins version
# $4: Path to packager config file
execute_cwp_jar() {

    if [ "$#" -gt 3 ]
    then
        java -jar "$2" -configPath "$4" -tmpDir "$1/out/tmp/" -version "$3"
        return 0
    else
        echo "Error. Missing parameters:"
        echo "   Working directory"
        echo "   Path to cwp.jar"
        echo "   Jenkins version"
        echo "   Path to packager config file"
        return 1
    fi

}

# function to generate the docker image using CWP using a downloaded jar
# $1: Working directory
# $2: Path to cwp.jar
# $3: Jenkins version
# $4: Path to packager config file
# $5: Tag for the docker image. "jenkins-experimental/${TEST_NAME}" by default or
# "jenkins-experimental/jenkinsfile-runner-test" if it is run from a setUp function
execute_cwp_jar_and_generate_docker_image() {

    if [ "$#" -gt 3 ]
    then
        if [ "$#" -eq 5 ]
        then
            image_tag="$5"
        else
            # shellcheck disable=SC2154
            test_name="${_shunit_test_}"
            if [ -z "$test_name" ]
            then
                image_tag="jenkins-experimental/jenkinsfile-runner-test"
            else
                image_tag="jenkins-experimental/$test_name"
            fi
        fi

        if execute_cwp_jar "$1" "$2" "$3" "$4"
        then
            docker build -t "$image_tag" -f "$1/out/tmp/output/Dockerfile" "$1/out/tmp/output/"
            return 0
        else
            return 1
        fi
    else
        echo "Error. Missing parameters:"
        echo "   Working directory"
        echo "   Path to cwp.jar"
        echo "   Jenkins version"
        echo "   Path to packager config file"
        echo "   (Optional) Tag for the docker image. 'jenkins-experimental/TEST_NAME' by default or 'jenkins-experimental/jenkinsfile-runner-test' if it is run from a setUp function"
        return 1
    fi

}

# function to generate the docker image using CWP downloading a downloaded jar
# $1: Working directory
# $2: CWP version to use
# $3: Jenkins version
# $4: Path to packager config file
# $5: Tag for the docker image. "jenkins-experimental/${TEST_NAME}" by default or
# "jenkins-experimental/jenkinsfile-runner-test" if it is run from a setUp function
download_execute_and_generate_docker_image_with_cwp() {

    if [ "$#" -gt 3 ]
    then
        cwp_jar_file=$(download_cwp "$1" "$2")
        if execute_cwp_jar_and_generate_docker_image "$1" "$cwp_jar_file" "$3" "$4" "$5"
        then
            return 0
        else
            return 1
        fi
    else
        echo "Error. Missing parameters:"
        echo "   Working directory"
        echo "   Path to cwp.jar"
        echo "   Jenkins version"
        echo "   Path to packager config file"
        echo "   (Optional) Tag for the docker image. 'jenkins-experimental/TEST_NAME' by default or 'jenkins-experimental/jenkinsfile-runner-test' if it is run from a setUp function"
        return 1
    fi

}

# function to generate the docker image using the CWP docker image
# $2: Path to packager config file
# $3: Tag for the docker image. "jenkins-experimental/${TEST_NAME}" by default or
# "jenkins-experimental/jenkinsfile-runner-test" if it is run from a setUp function
generate_docker_image_from_cwp_docker_image() {

    if [ "$#" -ge 1 ]
    then
        if [ "$#" -eq 2 ]
        then
            image_tag="$2"
        else
            if [ -z "${_shunit_test_}" ]
            then
                image_tag="jenkins-experimental/jenkinsfile-runner-test"
            else
                image_tag="jenkins-experimental/${_shunit_test_}"
            fi
        fi

        if [ -z "${_shunit_test_}" ]
        then
            # In case this function is not invoked from the test but from a setUp/tearDown function
            work_directory="/tmp/.cwp-docker"
        else
            work_directory="/tmp/${_shunit_test_}/.cwp-docker"
        fi

        mkdir -p "$work_directory"
        mkdir -p "$work_directory/tmp"

        docker pull jenkins/custom-war-packager
        docker run --rm -v "$work_directory/tmp":/tmp -v "$1":/sources/packager-config.yml jenkins/custom-war-packager -configPath /sources/packager-config.yml -tmpDir /tmp/app
        docker build -t "$image_tag" -f "$work_directory/tmp/app/output/Dockerfile" "$work_directory/tmp/app/output"
        return 0
    else
        echo "Error. Missing parameters:"
        echo "   Path to packager config file"
        echo "   (Optional) Tag for the docker image. 'jenkins-experimental/TEST_NAME' by default or 'jenkins-experimental/jenkinsfile-runner-test' if it is run from a setUp function"
        return 1
    fi

}
