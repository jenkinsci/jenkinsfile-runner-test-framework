#!/bin/bash
set -e

# Path to this source file
export parent_directory=$(dirname "$(pwd)")
export check_directory="$parent_directory/checksyntax/.checksyntax"

process_inc_file() {
  echo "Checking $1"
  output_file="$check_directory/${1##*/}.gcc"
  docker run -v "$parent_directory":/mnt koalaman/shellcheck:stable -f gcc "$1" > "$output_file"
  result=$(cat "$output_file")
  if [ -z "$result" ]
  then
    rm "$output_file"
  else
    echo "$result"
  fi
  echo ""
}

cd "$parent_directory"
rm -rf "$check_directory"
mkdir -p "$check_directory"

export -f process_inc_file
find . -name '*.inc' -exec bash -c 'process_inc_file {}' \;

echo "[INFO] A copy of the results can be found at $check_directory"
echo ""
