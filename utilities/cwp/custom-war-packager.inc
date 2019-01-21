# function to download the CWP jar
# $1: Working directory
# $2: CWP version to use
download_cwp() {

	if [ "$#" -gt 1 ]
	then
		cwp_jar_file="$1/.cwp-build/cwp-cli-$2.jar"

		rm -rf "$1/.cwp-build"
		mkdir -p "$1/.cwp-build"
		wget -O "$cwp_jar_file" https://repo.jenkins-ci.org/releases/io/jenkins/tools/custom-war-packager/custom-war-packager-cli/$2/custom-war-packager-cli-$2-jar-with-dependencies.jar

		echo "CWP downloaded as $cwp_jar_file"

		return 0
	else
		echo "Error. Missing parameters:"
		echo "   Working directory"
		echo "   CWP version to use"
		return 1
	fi

}

# function to generate the docker image using CWP using a downloaded jar
# $1: Working directory
# $2: Path to cwp.jar
# $3: Jenkins version
# $4: Path to packager config file
# $5: Tag for the docker image. "jenkins-experimental/jenkinsfile-runner-test" by default
generate_docker_image_from_cwp_jar() {

	if [ "$#" -gt 3 ]
	then
		if [ "$#" -eq 5 ]
		then
			image_tag="$5"
		else
			image_tag="jenkins-experimental/jenkinsfile-runner-test"
		fi

		java -jar $2 -configPath $4 -tmpDir "$1/out/tmp/" -version $3
		docker build -t $image_tag -f "$1/out/tmp/output/Dockerfile" "$1/out/tmp/output/"

		return 0
	else
		echo "Error. Missing parameters:"
		echo "   Working directory"
		echo "   Path to cwp.jar"
		echo "   Jenkins version"
		echo "   Path to packager config file"
		echo "   Tag for the docker image. 'jenkins-experimental/jenkinsfile-runner-test' by default"
		return 1
	fi

}

# function to generate the docker image using CWP
# $1: Working directory
# $2: CWP version to use
# $3: Jenkins version
# $4: Path to packager config file
# $5: Tag for the docker image. "jenkins-experimental/jenkinsfile-runner-test" by default
generate_docker_image() {

	if [ "$#" -gt 3 ]
	then
		cwp_jar_file=$(download_cwp $1 $2)
		cwp_jar_file=$(remove_string "$cwp_jar_file" "CWP downloaded as ")

		generate_docker_image_from_jar $1 $cwp_jar_file $3 $4 $5

		return 0
	else
		echo "Error. Missing parameters:"
		echo "   Working directory"
		echo "   Path to cwp.jar"
		echo "   Jenkins version"
		echo "   Path to packager config file"
		echo "   Tag for the docker image. 'jenkins-experimental/jenkinsfile-runner-test' by default"
		return 1
	fi

}
