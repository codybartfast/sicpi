#include "explicit-control-evaluator.h"
#include "dialect.h"
#include "lexer.h"
#include "metacircular-procedures.h"
#include "parser.h"
#include "run.h"
#include "sicp-error.h"

object run(object program)
{
	set_dialect(the_global_environment(false));
	return EC_Eval(program);
}

object load_run(source src)
{
	object expr = parse_source(src);

	if (expr == EMPTY_LIST) {
		return VOID_VALUE;
	}

	object program =
		cons(of_name("print-lines", NO_META_DATA), expr, NO_META_DATA);
	object rslt = run(program);
	return rslt;
}

object parse_source(source src)
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
