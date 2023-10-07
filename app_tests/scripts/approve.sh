#!/usr/bin/env bash

#############################
###   Check Environment   ###
#############################

rpath(){
	realpath --relative-to="." "$1"
}

script_dir=$(rpath "$(dirname -- "$0")")

test_dir=$(rpath "$script_dir/..")
data_dir=$(rpath "$test_dir/data")
base_dir=$(rpath "$test_dir/..")
code_dir=$(rpath "$base_dir/code")
sicp_bin=$(rpath "$base_dir/bin/sicp")

echo "Using '$base_dir' as base directory."

if [[ ! -e "$sicp_bin" ]] ;
then
	echo "FATAL: Did not find sicp executable at '$(rpath "$sicp_bin")' ($sicp_bin)."
	exit 1
fi
echo "Found sicp executable: '$(rpath "$sicp_bin")'."

if [[ ! -d "$code_dir" ]] ;
then
	echo "FATAL: Did not find code directory at '$(rpath "$code_dir")' ($code_dir)."
	exit 1
fi
echo "Found code directory: '$(rpath "$code_dir")'."

#####################
###   Functions   ###
#####################

test(){
	local source="$1"
	echo -n "$source "

	local rel_path
	rel_path=$(realpath --relative-to="$code_dir" "$source")

	local app_source="$data_dir/$rel_path"
	mkdir -p "$(dirname -- "$app_source")"
	touch "$app_source"

	local expected="$data_dir/$rel_path.approved"
	touch "$expected"

	local volatile="$data_dir/$rel_path.volatile"

	local actual="$data_dir/$rel_path.actual"
	"$sicp_bin" < "$source" > "$actual" 2>&1

	if cmp -s "$expected" "$actual" ;
	then
		echo "✓"
	elif [[ -e $volatile ]] ;
	then
		if [[ "$(blur "$expected")" == "$(blur "$actual")" ]] ;
		then
			echo "~"
		else
			review </dev/tty;
		fi
	else
		review </dev/tty;
	fi
}

blur(){
    	local pattern='[[:digit:]]\+\(\.[[:digit:]]\+\)\?\(e[-+][[:digit:]]\+\)\?'
	sed "s/$pattern/<volatile>/g" < "$1"
}

review(){
	echo
	line "V"
	echo "Changes in '$source'"
	echo
	diff "$app_source" "$source"

	echo
	line "."
	echo "Changes in output"
	echo
	diff "$expected" "$actual"

	echo
	line "-"
	echo "$source"
	echo

	declare -A option_names=( ["a"]="Approve" ["v"]="mark as Volatile" )
	local got_response=false;
	while [[ "$got_response" == false ]] ; do
		read -p "Approve, Skip or mark Volatile? " -r -n 1 choice
		echo
		if [[ "$choice" =~ ^[av]$ ]] ;
		then
			read -p "Definitely want to ${option_names[$choice]}? " -r -n 1 definitely
			echo
			if [[ "$definitely" == "d" ]] ;
			then
				case "$choice" in
					a)
				cp "$source" "$app_source"
				cp "$actual" "$expected"
				got_response=true
					;;
					v)
				touch "$volatile"
				got_response=true
					;;
				esac
			fi
		elif [[ "$choice" == "s" ]] ;
		then
			echo "Skipping"
			got_response=true
		fi
	done

	line '='
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

if [[  0 -eq "$code_file_count" ]] ;
then
	echo "EXITING: no code files found."
	exit 1
fi
echo "Found $code_file_count code files."

echo "$code_files" | while read -r code_file ;
do
   test "$code_file"
done
echo "Done"
