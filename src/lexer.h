#ifndef LEXER_H
#define LEXER_H

#include "source.h"
#include "string-builder.h"
#include "token.h"

typedef struct lexer {
	source source;
	string_builder text;
	string_builder temp;
	bool is_errored;
} *lexer;

lexer lexer_new(source src);
token lexer_read(lexer lxr);
bool lexer_is_errored(lexer lxr);
void lexer_free_source(lexer lxr);
void lexer_free(lexer lxr);
void lexer_set_token_source(lexer lxr, token_source *tkn_src);

#endif
