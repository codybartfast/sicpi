#ifndef TOKEN_H
#define TOKEN_H

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

typedef struct token {
	string_builder sb;
	enum token_type type;
	char *text;
	char *mesage;
	long offset;
	long x; // zero-based column
	long y; // zero-based line
} *token;

#endif
