#!/usr/bin/env bash

#############################
###   Check Environment   ###
#############################

script_dir=$(dirname -- "$0")
test_dir="$script_dir/.."
base_dir="$test_dir/.."
code_dir="$base_dir/code"
sicp_bin="$base_dir/bin/sicp"

rpath(){
	readlink -f -- "$1'"
}

echo "Using '$(rpath "$base_dir")' as base directory."

if [[ ! -e "$sicp_bin" ]]; then
	echo "FATAL: Did not find sicp executable at '$(rpath "$sicp_bin")' ($sicp_bin)."
	exit 1
fi
echo "Found sicp executable: '$(rpath "$sicp_bin")'."

if [[ ! -d "$code_dir" ]]; then
	echo "FATAL: Did not find code directory at '$(rpath "$code_dir")' ($code_dir)."
	exit 1
fi
echo "Found code directory: '$(rpath "$code_dir")'."

#####################
###   Functions   ###
#####################

test(){
	echo "Got: $1"
}

###############
###   Run   ###
###############

code_files=$(find "$code_dir" -type f -name "*.sicp" | sort)
code_file_count=$(echo "$code_files" | wc -l)

if [[  0 -eq "$code_file_count" ]]; then
	echo "EXITING: no code files found."
	exit 1
fi
echo "Found $code_file_count code files."

echo "$code_files" | while read line ; do
   test $line
done
