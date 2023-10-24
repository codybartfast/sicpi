rpath(){
	realpath --relative-to="." "$1"
}

script_dir=$(rpath "$(dirname -- "$0")")
emcc=$(rpath "$script_dir/emsdk/upstream/emscripten/emcc")
src_dir=$(rpath "$script_dir/../src")
out_dir=$(rpath "$script_dir/out")

mkdir -p "$out_dir"

$emcc							\
	-o "$out_dir/sicp.html"				\
	-sALLOW_MEMORY_GROWTH				\
							\
	"$src_dir/args.c"				\
	"$src_dir/character-classes.c"			\
	"$src_dir/dialect.c"				\
	"$src_dir/explicit-control-evaluator.c"		\
	"$src_dir/hardware.c"				\
	"$src_dir/lexer.c"				\
	"$src_dir/list.c"				\
	"$src_dir/metacircular-procedures.c"		\
	"$src_dir/obarray.c"				\
	"$src_dir/object.c"				\
	"$src_dir/parser.c"				\
	"$src_dir/primitive-procedures.c"		\
	"$src_dir/run.c"				\
	"$src_dir/sicp.c"				\
	"$src_dir/sicp-error.c"				\
	"$src_dir/sicp-std.c"				\
	"$src_dir/source.c"				\
	"$src_dir/string-builder.c"			\
	"$src_dir/token.c"				\
	"$src_dir/token-table.c"			\

