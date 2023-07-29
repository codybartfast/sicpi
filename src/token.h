#ifndef TOKEN_H
#define TOKEN_H

#include "string-builder.h"

typedef struct lexer *lexer;

typedef enum token_type {
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
} token_type;

typedef struct token {
	lexer lxr;
	enum token_type type;
	char *text;
	char *err_msg;
	long offset;
	long x; // zero-based column
	long y; // zero-based line
} *token;

token_type tkn_type(token tkn);
char *tkn_text(token tkn);
char *tkn_err_msg(token tkn);
long tkn_offset(token tkn);
long tkn_x(token tkn);
long tkn_y(token tkn);

typedef struct token_source {
	void *state;
	token (*read_token)(void *state);
} token_source;

#endif
