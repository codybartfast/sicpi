#!/usr/bin/env bash

script_dir=$(dirname -- "0")
test_dir="$script_dir/.."
repo_root="$test_dir/.."
code_dir="$repo_root/code"
sicp_bin="$repo_root/bin/sicp"

echo "Using '$(readlink -f -- "$repo_root")' as root directory."

if [[ ! -e "$sicp_bin" ]]; then
	echo "FATAL: Did not find sicp executable at '$(readlink -f -- "$sicp_bin")' ($sicp_bin)."
	exit 1
fi
echo "Found sicp executable: '$(readlink -f -- "$sicp_bin")'."

if [[ ! -d "$code_dir" ]]; then
	echo "FATAL: Did not find code directory at '$(readlink -f -- "$code_dir")' ($code_dir)."
	exit 1
fi
echo "Found code directory: '$(readlink -f -- "$code_dir")'."

code_files=$(find "$code_dir" -type f -name "*.sicp" -printf '%p\n')
code_file_count=$(echo "$code_files" | wc -l)

if [[  0 -eq "$code_file_count" ]]; then
	echo "EXITING: no code files found."
	exit 1
fi
echo "Found $code_file_count code files."
