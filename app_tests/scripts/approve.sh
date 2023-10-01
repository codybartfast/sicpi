#!/usr/bin/env bash

#############################
###   Check Environment   ###
#############################

# todo: catch stderr

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

test(){
	source="$1"
	echo -n "$source"

	rel_path=$(realpath --relative-to="$code_dir" "$source")

	app_source="$data_dir/$rel_path"
	mkdir -p "$(dirname -- "$app_source")"
	touch "$app_source"

	expected="$data_dir/$rel_path.approved"
	touch "$expected"

	actual="$data_dir/$rel_path.actual"
	"$sicp_bin" < "$source" > "$actual"

	if cmp -s "$expected" "$actual" ; then
		echo "  ✓"
	else
		echo " ..."
		review </dev/tty;
	fi
}


review(){
	echo
	line "="
	echo "Changes in '$source'"
	# line "."
	echo
	diff "$app_source" "$source"

	echo
	line "."
	echo "Changes in output"
	# line "."
	echo
	diff "$expected" "$actual"

	echo
	line "-"
	echo "$source"
	echo

	got_response=0;
	while [[ "$got_response" == 0 ]] ; do
		read -p "Approve or Skip? " -r -n 1
		echo
		if [[ "$REPLY" == "a" ]] ; then
			read -p "Definitely? " -r -n 1
			echo
			if [[ "$REPLY" == "d" ]] ; then
				cp $source $app_source
				cp $actual $expected
				got_response=1
			fi
		elif [[ "$REPLY" == "s" ]] ; then
			echo "Skipping"
			got_response=1
		fi
	done

	line 'V'
	echo

}

line(){
	printf "%0.s$1" {1..80}
	echo
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

