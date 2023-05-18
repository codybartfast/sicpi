#ifndef LEXER_H
#define LEXER_H

#include "source.h"
#include "string-builder.h"

typedef struct lexer {
	source source;
	string_builder sb;
} *lexer;

#endif
