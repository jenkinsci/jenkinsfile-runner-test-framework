/* Only keep the 10 most recent builds. */
properties([[$class: 'BuildDiscarderProperty',
                strategy: [$class: 'LogRotator', numToKeepStr: '10']]])

node {
    stage('checkout') {
        deleteDir()
        checkout scm
    }
    
    stage('shellcheck') {
        dir('jenkinsfile-runner-test-framework') {
            global_check_result=0
            current_directory="${WORKSPACE}/jenkinsfile-runner-test-framework"
            def incFiles=findFiles(glob: '**/*.inc')
            incFiles.each{
                if("${it}".lastIndexOf("/") == -1) {
                    output_file="${it}.json"
                } else {
                    output_file="${it}".substring("${it}".lastIndexOf("/")+1) + ".json"
                }
                result=sh (script: "docker run -v $current_directory:/mnt koalaman/shellcheck:stable -f json ${it} > $output_file", returnStatus: true)
                if(result) {
                    global_check_result++
                }
            }
            if(global_check_result > 0) {
                echo "One or more scripts have an invalid sintaxis"
                currentBuild.result = 'UNSTABLE'
                archiveArtifacts artifacts: '**/*.inc.json'
            }
        }
    }

    stage('testing') {
        dir('jenkinsfile-runner-test-framework') {
            result=sh (script: 'make test', returnStatus: true) != 0
            if(result) {
                echo "Test failures!"
                currentBuild.result = 'FAILED'
            }
        }
    }
}
