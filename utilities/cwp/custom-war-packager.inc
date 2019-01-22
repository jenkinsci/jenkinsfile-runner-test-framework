# function to download the CWP jar
# $1: Working directory
# $2: CWP version to use
download_cwp() {

	if [ "$#" -gt 1 ]
	then
		cwp_jar_file="$1/.cwp-build/cwp-cli-$2.jar"

		rm -rf "$1/.cwp-build"
		mkdir -p "$1/.cwp-build"

		if [[ $2 == *"SNAPSHOT"* ]]
		then
			wget -O "$cwp_jar_file" https://repo.jenkins-ci.org/snapshots/io/jenkins/tools/custom-war-packager/custom-war-packager-cli/$2/custom-war-packager-cli-$2-jar-with-dependencies.jar
		else
			if [[ $2 == *"rc"* ]]
			then
				wget -O "$cwp_jar_file" https://repo.jenkins-ci.org/incrementals/io/jenkins/tools/custom-war-packager/custom-war-packager-cli/$2/custom-war-packager-cli-$2-jar-with-dependencies.jar
			else
				wget -O "$cwp_jar_file" https://repo.jenkins-ci.org/releases/io/jenkins/tools/custom-war-packager/custom-war-packager-cli/$2/custom-war-packager-cli-$2-jar-with-dependencies.jar
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
		echo "   CWP version to use"
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
		java -jar $2 -configPath $4 -tmpDir "$1/out/tmp/" -version $3
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
			if [ -z "${_shunit_test_}" ]
			then
				image_tag="jenkins-experimental/jenkinsfile-runner-test"
			else
				image_tag="jenkins-experimental/${_shunit_test_}"
			fi
		fi

		result=$(execute_cwp_jar $1 $2 $3 $4)
		if [ "$?" -eq "0" ]
		then
			docker build -t $image_tag -f "$1/out/tmp/output/Dockerfile" "$1/out/tmp/output/"
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
		echo "   Tag for the docker image. 'jenkins-experimental/TEST_NAME' by default or 'jenkins-experimental/jenkinsfile-runner-test' if it is run from a setUp function"
		return 1
	fi

}

# function to generate the docker image using CWP
# $1: Working directory
# $2: CWP version to use
# $3: Jenkins version
# $4: Path to packager config file
# $5: Tag for the docker image. "jenkins-experimental/${TEST_NAME}" by default or
# "jenkins-experimental/jenkinsfile-runner-test" if it is run from a setUp function
download_execute_and_generate_docker_image_with_cwp() {

	if [ "$#" -gt 3 ]
	then
		cwp_jar_file=$(download_cwp $1 $2)
		if [ "$?" -eq "0" ]
		then
			execute_cwp_jar_and_generate_docker_image $1 $cwp_jar_file $3 $4 $5
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
		echo "   Tag for the docker image. 'jenkins-experimental/TEST_NAME' by default or 'jenkins-experimental/jenkinsfile-runner-test' if it is run from a setUp function"
		return 1
	fi

}
