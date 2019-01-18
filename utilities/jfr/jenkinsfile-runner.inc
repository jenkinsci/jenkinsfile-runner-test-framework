# function to run a docker image
# $1: tag of the docker image
# $2: Path to Jenkinsfile
run_jfr_docker_image() {

	if [ "$#" -gt 1 ]
	then
        docker run --rm -v $2:/workspace/Jenkinsfile $1
		return 0
	else
		echo "Error. Missing tag for the docker image to run"
		echo "Error. Missing parameters:"
		echo "   Tag for the docker image to run"
		echo "   Path to Jenkinsfile"
		return 1
	fi

}