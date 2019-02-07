#!/bin/bash
set -e

# 5 minutes by default
default_timeout=300
export TIMEOUT=$default_timeout

# 
create_test_file() {

    if [ "$#" -eq 1 ]
    then
	    rm -f "$1"
	    echo "#!/bin/bash" >> "$1"
        return 0
    else
        echo "Error. Missing the parameter for the test file to create."
        return 1
    fi
}

# Set the timeout.
# $1: timeout in seconds. A negative value is not allowed. Set 0 to disable the timeout and -1 to reset to default value
set_timeout() {

    if [ "$#" -eq 1 ]
    then
        if [ "$1" -gt 0 ]
        then
            export TIMEOUT=$1
            return 0
        elif [ "$1" -eq 0 ]
        then
            unset TIMEOUT
            return 0
        elif [ "$1" -eq -1 ]
        then
            export TIMEOUT=$default_timeout
            return 0
        else
            echo "[ERROR] Wrong timeout value. It must be a positive integer, 0 to disable or -1 to reset to default value"
            return 1
        fi
    else
        echo "Error. Wrong number of parameters. Use timeout timeout_value"
        echo "   The timeout value is a positive integer. Use '0' to disable the timeout and '-1' to reset to default value"
        return 1
    fi

}

# Generate a test that is executed with or without timeout based on the original test
# $1: Name of the original test
# $2: File where create the test (full path)
create_test_with_template() {

    if [ "$#" -eq 2 ]
    then
        test_name="$1_with_timeout"
        {
            echo "";
            echo "$test_name() {";
            echo "  if [ -z \"\$TIMEOUT\" ]";
            echo "  then";
            echo "    $1";
            echo "  else";
            echo "    run_with_timeout $1"
            echo "  fi"
            echo "}"
            echo ""
        } >> "$2"
        echo "$test_name"
    else
        echo "Error. Missing parameters:"
        echo "   Name of the original test"
        echo "   File where create the test (full path)"
        return 1
    fi

}

# Execute a function setting a timeout. If the timeout happens then the execution of the function is killed and a message is displayed
# $1: name of the fucntion to execute
run_with_timeout() {

    cmd="$1"
    timeout="$TIMEOUT"
    
    ( 
        eval "$cmd" &
        child=$!
        trap -- "" SIGTERM 
        (
                sleep $timeout
                # Only kills and displays message if the function is still running
                # shellcheck disable=SC2009
                if [ "$(ps -p $child | grep -cv PID )" -ne 0 ]
                then 
                    echo "[ERROR] $1 timeout. Passed $timeout seconds"
                    kill_process_and_subprocess $child
                fi
        ) &     
        wait $child
    )

}

# Kill the process and all the subprocesses that it can open
# $1: PID of the process to kill
kill_process_and_subprocess() {

    for subprocess in $(pgrep -P "$1")
    do
        # Kill the subprocess
        kill_process_and_subprocess "$subprocess"
    done
    # Kill execution
    kill "$1" 2> /dev/null 

}
