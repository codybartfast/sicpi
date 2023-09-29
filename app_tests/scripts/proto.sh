#!/usr/bin/env bash

#############################
###   Check Environment   ###
#############################

rpath(){
	# readlink -f -- "$1"
	realpath --relative-to="." "$1"
}

script_dir=$(rpath "$(dirname -- "$0")")

test_dir=$(rpath "$script_dir/..")
data_dir=$(rpath "$test_dir/data")
base_dir=$(rpath "$test_dir/..")
code_dir=$(rpath "$base_dir/code")
sicp_bin=$(rpath "$base_dir/bin/sicp")

echo "Using '$base_dir' as base directory."

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

make_containing_dir(){
	mkdir -p "$(dirname -- "$1")"
}

test(){
	source="$1"
	echo "Doing $source"
	rel_path=$(realpath --relative-to="$code_dir" "$source")
	actual="$data_dir/$rel_path.actual"

	make_containing_dir "$actual"

	"$sicp_bin" < "$source" > "$actual"
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

echo "$code_files" | while read -r line ; do
   test "$line"
done
