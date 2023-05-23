#ifndef LEXER_H
#define LEXER_H

#include "source.h"
#include "string-builder.h"

typedef enum token_type {
	TKN_ERROR = -2,
	TKN_EOF = -1,
	TKN_UNDEFINED = 0,
	// TKN_COMMENT = 1
	// TKN_IDENTIFIER,
	// TKN_LIST_OPEN,
	// TKN_LIST_CLOSE,
	// TKN_NUMBER,
	// TKN_STRING,
	// TKN_DOT,
	// TKN_QUOTE,
	// TKN_QUASIQUOTE,
	// TKN_UNQUOTE
} token_type;

typedef struct lexer *lexer;

typedef struct token {
	lexer lexer;
	enum token_type type;
	char *text;
	char *mesage;
	long offset;
	long x; // zero-based column 
	long y; // zero-based line
} *token;

struct lexer {
	source source;
	string_builder sb;
};

token lexer_read(lexer lxr);
void lexer_free(void);

#endif
