#ifndef LEXER_H
#define LEXER_H

#include "source.h"
#include "string-builder.h"
#include "token.h"

typedef struct lexer {
	source source;
	string_builder sb;
} *lexer;

lexer lexer_new(source src);
token lexer_read(lexer lxr);
void lexer_free(void);

#endif
