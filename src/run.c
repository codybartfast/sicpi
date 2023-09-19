#include "sicp-error.h"
#include "explicit-control-evaluator.h"
#include "lexer.h"
#include "parser.h"
#include "run.h"

object load(source src)
{
	struct token_source tkn_src;
	struct parser parser;

	lexer lxr = lexer_new(src);
	lexer_set_token_source(lxr, &tkn_src);
	parser_init(&parser, &tkn_src);

	object program = parse_all(&parser);
	if (is_error(program)) {
		eprintfx(to_text(program));
	}

	lexer_free(lxr);
	return program;
}

object run(object program)
{
	return EC_Eval(program);
}

object load_run(source src)
{
	object expr = load(src);
	object program = cons(BEGIN, expr, NO_META_DATA);
	return run(program);
}
