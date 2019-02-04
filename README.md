# Jenkinsfile Runner Test Framework
This repository provides a test harness for verifying Jenkinsfile Runner Docker images. That image can be already built or it can be built using Custom War Packager and the set of functions provided by the framework.

The framework takes a Jenkinsfile from a directory and runs against the image, waiting for its completion, and then verifies log outputs and results.

[CHANGELOG](./CHANGELOG.md)

## Technical requirements
The test suite is designed to run on Linux, targeting JFR execution only in Docker containers.
It relies on shUnit2 and its hooks allow to check the log outputs, the content in the workspace and the result of the execution in an easy way.

In case it's desired to build the docker image using the Custom War Package it is possible to specify a concrete version (released, timestamped snapshot or incremental), although the framework defines a default version.

The test framework supports timeouts (_in progress at the time of writing this README file_) and `JAVA_OPTS` environment variable.

## Usage
### Enable the framework
To make use of the test framework we have to download the scripts cloning the repository or downloading the sources. Then execute the Makefile provided by the framework. An example might be:

```
# Basic Makefile example
.PHONY: all

all: clean init test

clean:
    # Avoid to cache the framework
	rm -rf jenkinsfile-runner-test-framework

init:
    # Retrieve the test framework. In this case, we are cloning the repository
	git clone --depth 1 https://github.com/jenkinsci/jenkinsfile-runner-test-framework
	$(MAKE) -C jenkinsfile-runner-test-framework

test:
# Execute your tests here
```

The Makefile to init the framework will download the shUnit2 library so any project/developer making use of this framework can disregard that step

### Init the framework in the test script

Jenkinsfile Runner Test Framework unifies all the global configuration and the files to include in a single script `init-jfr-test-framework.inc`, so we just have to invoke and load it.

```
test_framework_directory="path_to_jenkinsfile_runner_test_framework"

. $test_framework_directory/init-jfr-test-framework.inc
```

Once the test framework is loaded, all the functions will be available as any other shell script function.

```
test_example_that_download_CWP_jar_generate_docker_image_and_run_jenkinsfile() {
    downloaded_cwp_jar=$(download_cwp "$test_framework_directory")
    
    jfr_tag=$(execute_cwp_jar_and_generate_docker_image "$test_framework_directory" "$downloaded_cwp_jar" "$version" "path_to/packager-config.yml" "$jenkinsfile_runner_tag" | grep 'Successfully tagged')
        
    execution_should_success "$?" "$jfr_tag" "$jenkinsfile_runner_tag"

    result=$(run_jfr_docker_image "$jenkinsfile_runner_tag" "path_to/Jenkinsfile")
    
    jenkinsfile_execution_should_succed "$?" "$result"
}
```

The last step is to init all the framework invoking the `init_framework` function, which is defined in the `init-jfr-test-framework.inc` script. This function *must be invoked at the end of the test shell script so shUnit2 is loaded properly*.

## Contributing
In case someone is interested on contributing, the Jenkinsfile Runner Test Framework gives the opportunity to check any change:

* In case the contributor would like to check the syntax of the scripts, it's enough to execute `make syntax`
* In case the desire is to execute the smoke tests, the execution command is `make test`
* The recommendation is to check and verify both texts and syntax. In that case, just execute `make verify`

## Further reading

* [Jenkinsfile Runner](https://github.com/jenkinsci/jenkinsfile-runner/)
* [A unit test framework for shell scripts](https://github.com/kward/shunit2)
