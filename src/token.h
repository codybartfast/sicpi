#ifndef TOKEN_H
#define TOKEN_H

#include "string-builder.h"

enum token_type {
	TOKEN_ERROR = -2,
	TOKEN_EOF = -1,
	TOKEN_UNDEFINED = 0,
	TOKEN_IDENTIFIER, // 1
	TOKEN_LIST_OPEN, /// 2
	TOKEN_LIST_CLOSE, // 3
	TOKEN_NUMBER, ////// 4
	TOKEN_STRING, ////// 5
	TOKEN_DOT, ///////// 6
	TOKEN_QUOTE, /////// 7
	TOKEN_QUASIQUOTE, // 8
	TOKEN_UNQUOTE, ///// 9
};

typedef struct token {
	void *meta_data;
	enum token_type type;
	char *text;
	long offset;
	long x; // zero-based column
	long y; // zero-based line
} *token;

enum token_type token_type(token tkn);
char *token_text(token tkn);
// char *token_error_message(token tkn);
long token_offset(token tkn);
long token_x(token tkn);
long token_y(token tkn);
void *token_metadata(token tkn);

typedef struct token_source {
	void *state;
	token (*read_token)(void *state);
} token_source;

#endif
