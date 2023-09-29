#include "run.h"

#include "dialect.h"
#include "explicit-control-evaluator.h"
#include "lexer.h"
#include "metacircular-procedures.h"
#include "parser.h"
#include "sicp-error.h"

object run(object program)
{
	set_dialect(the_global_environment(false));

	object rslt = VOID_VALUE;
	for (; program != EMPTY_LIST && !is_error(rslt);
	     program = cdr(program)) {
		rslt = EC_Eval(car(program));
		if (rslt != OK) {
			printf("%s\n", to_text(rslt));
		}
	}
	return rslt;
}

object load_run(source src)
{
	object program = parse_source(src);
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
