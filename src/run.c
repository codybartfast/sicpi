#include "run.h"

#include "dialect.h"
#include "explicit-control-evaluator.h"
#include "lexer.h"
#include "metacircular-procedures.h"
#include "parser.h"
#include "sicp-error.h"

object run(source src)
{
	struct token_source tkn_src;
	struct parser parser;

	lexer lxr = lexer_new(src);
	lexer_set_token_source(&tkn_src, lxr);
	parser_init(&parser, &tkn_src);

	set_dialect(the_global_environment(false));

	object exp;
	object rslt = VOID_VALUE;
	while ((exp = parse(&parser)) != EOS) {
		RETURN_IF_ERROR(exp);
		rslt = EC_Eval(exp);
		if (rslt != OK) {
			printf("%s\n", to_text(rslt));
		}
		RETURN_IF_ERROR(rslt);
	}
	lexer_free(lxr);
	printf("\n");
	return rslt;
}

object parse_source(source src)
{
	struct token_source tkn_src;
	struct parser parser;

	lexer lxr = lexer_new(src);
	lexer_set_token_source(&tkn_src, lxr);
	parser_init(&parser, &tkn_src);

	object program = parse_all(&parser);
	if (is_error(program)) {
		eprintfx(to_text(program));
	}

	lexer_free(lxr);
	return program;
}
