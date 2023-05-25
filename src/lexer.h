#ifndef LEXER_H
#define LEXER_H

#include "source.h"
#include "string-builder.h"
#include "token.h"

typedef struct lexer {
	source source;
	string_builder text;
	string_builder temp;
} *lexer;

lexer lexer_new(source src);
token lexer_read(lexer lxr);
void lexer_free(lexer lxr);

token_type tkn_type(token tkn);
char *tkn_text(token tkn);

#endif
