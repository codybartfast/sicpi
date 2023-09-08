#include "sicp-error.h"
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
	object rslt = parse_all(&parser);
	if (is_error(rslt)) {
		eprintfx(to_text(rslt));
	}
	lexer_free(lxr);
	return rslt;
}

object run(object program)
{
	UNUSED(program);
	return of_string(to_text(program), NO_META_DATA);
}
