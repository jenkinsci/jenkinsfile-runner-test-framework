/* Only keep the 10 most recent builds. */
properties([[$class: 'BuildDiscarderProperty',
                strategy: [$class: 'LogRotator', numToKeepStr: '10']]])

node {
    stage('checkout') {
        deleteDir()
        checkout scm
    }

    stage('sanity check') {
        withEnv(["PATH=${env.WORKSPACE}/checksyntax:${env.PATH}"]) {
            dir('jenkinsfile-runner-test-framework') {
                warnError {
                    def reportFileName = 'pre-commit.out'
                    sh """
                      curl https://pre-commit.com/install-local.py | python -
                      git diff-tree --no-commit-id --name-only -r \$(git rev-parse HEAD) | xargs pre-commit run --files | tee ${reportFileName}
                    """
                    archiveArtifacts artifacts: reportFileName
                }
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
