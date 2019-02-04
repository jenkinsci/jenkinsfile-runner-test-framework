#!/bin/bash
set -e

# Return the workspace
# $1: (Optional) Workspace directory. If not specified, 'WORKSPACE' env variable must be defined.
# If the env variable is set, this parameter will override it
read_workspace() {

    if [ "$#" -eq 1 ]
    then
        if [ -z "$1" ]
        then
            if [ -z "$WORKSPACE" ]
            then
                workspace_dir=""
            else
                workspace_dir="$WORKSPACE"
            fi
        else
            workspace_dir="$1"
        fi
    else
        if [ -z "$WORKSPACE" ]
        then
            workspace_dir=""
        else
            workspace_dir="$WORKSPACE"
        fi
    fi
    echo "$workspace_dir"

}

# Check if a directory exists
# $1: (Optional) Workspace directory. If not specified, 'WORKSPACE' env variable must be defined.
# If the env variable is set, this parameter will override it
workspace_exists() {

    workspace_dir=$(read_workspace "$1")
    assertNotEquals "Workspace is not configured. Please set the WORKSPACE environment variable or pass it as parameter" "" "$workspace_dir"

    if [ -d "$workspace_dir" ]
    then
        valid_folder=1
    else
        valid_folder=0
    fi
    assertEquals "$workspace_dir is not a valid folder" "1" "$valid_folder"

    return 0

}

# Check if a directory does not exists
# $1: (Optional) Workspace directory. If not specified, 'WORKSPACE' env variable must be defined.
# If the env variable is set, this parameter will override it
workspace_does_not_exist() {

    workspace_dir=$(read_workspace "$1")
    assertNotEquals "Workspace is not configured. Please set the WORKSPACE environment variable or pass it as parameter" "" "$workspace_dir"

    if [ -d "$workspace_dir" ]
    then
        valid_folder=1
    else
        valid_folder=0
    fi
    assertEquals "$workspace_dir should not be a valid folder" "0" "$valid_folder"

    return 0

}

# Check if a file exists in the workspace
# $1: File to check the existance
# $2: (Optional) Workspace directory. If not specified, 'WORKSPACE' env variable must be defined.
# If the env variable is set, this parameter will override it
file_exists_in_workspace() {

    if [ "$#" -ge 1 ]
    then
        workspace_dir=$(read_workspace "$2")
        workspace_exists "$workspace_dir"

        if [ -e "$workspace_dir/$1" ]
        then
            existing_file=1
        else
            existing_file=0
        fi
        assertEquals "Not found $workspace_dir/$1" "1" "$existing_file"

        return 0
    else
        echo "Error. Missing parameters:"
        echo "   File to check the existance"
        echo "   (Optional) Workspace directory. If not specified, 'WORKSPACE' env variable must be defined"
        return 1
    fi

}

# Check if a file does not exist in the workspace
# $1: File to check the existance
# $2: (Optional) Workspace directory. If not specified, 'WORKSPACE' env variable must be defined.
# If the env variable is set, this parameter will override it
file_does_not_exist_in_workspace() {

    if [ "$#" -ge 1 ]
    then
        workspace_dir=$(read_workspace "$2")
        workspace_exists "$workspace_dir"

        if [ -e "$workspace_dir/$1" ]
        then
            existing_file=1
        else
            existing_file=0
        fi
        assertEquals "Found $workspace_dir/$1" "0" "$existing_file"

        return 0
    else
        echo "Error. Missing parameters:"
        echo "   File to check the existance"
        echo "   (Optional) Workspace directory. If not specified, 'WORKSPACE' env variable must be defined"
        return 1
    fi

}

# Check if a file contains a text
# $1: Text to check
# $2: File to check
# $3: (Optional) Workspace directory. If not specified, 'WORKSPACE' env variable must be defined.
# If the env variable is set, this parameter will override it
file_contains_text() {

    if [ "$#" -ge 2 ]
    then
        workspace_dir=$(read_workspace "$3")
        workspace_exists "$workspace_dir"

        file_exists_in_workspace "$2" "$workspace_dir"

        if grep -q "$1" "$workspace_dir/$2"
        then
            contains_text=1
        else
            contains_text=0
        fi
        assertEquals "Expected text '$1' not found in $2" "1" "$contains_text"

        return 0
    else
        echo "Error. Missing parameters:"
        echo "   Text to check"
        echo "   File to check"
        echo "   (Optional) Workspace directory. If not specified, 'WORKSPACE' env variable must be defined"
        return 1
    fi

}

# Check if a file does not contains a test
# $1: Text to check
# $2: File to check
# $3: (Optional) Workspace directory. If not specified, 'WORKSPACE' env variable must be defined.
# If the env variable is set, this parameter will override it
file_does_not_contains_text() {

    if [ "$#" -ge 2 ]
    then
        workspace_dir=$(read_workspace "$3")
        workspace_exists "$workspace_dir"

        file_exists_in_workspace "$2" "$workspace_dir"

        if grep -q "$1" "$workspace_dir/$2"
        then
            contains_text=1
        else
            contains_text=0
        fi
        assertEquals "Unexpected text '$1' found in $2" "0" "$contains_text"

        return 0
    else
        echo "Error. Missing parameters:"
        echo "   Text to check"
        echo "   File to check"
        echo "   (Optional) Workspace directory. If not specified, 'WORKSPACE' env variable must be defined"
        return 1
    fi

}
