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
	lexer_set_token_source(lxr, &tkn_src);
	parser_init(&parser, &tkn_src);

	set_dialect(the_global_environment(false));

	object exp;
	object rslt = VOID_VALUE;
	while ((exp = parse(&parser)) != EOS) {
		RETURN_IF_ERROR(exp);
		RETURN_IF_ERROR(rslt = EC_Eval(exp));
		if (rslt != OK) {
			printf("%s\n", to_text(rslt));
		}
	}
	lexer_free(lxr);
	return rslt;
}
