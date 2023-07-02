#ifndef TOKEN_H
#define TOKEN_H

#include "string-builder.h"

typedef struct lexer *lexer;

typedef enum token_type {
	TKN_ERROR = -2,
	TKN_EOF = -1,
	TKN_UNDEFINED = 0,
	TKN_IDENTIFIER, // 1
	TKN_LIST_OPEN, /// 2
	TKN_LIST_CLOSE, // 3
	TKN_NUMBER, ////// 4
	TKN_STRING, ////// 5
	TKN_DOT, ///////// 6
	TKN_QUOTE, /////// 7
	TKN_QUASIQUOTE, // 8
	TKN_UNQUOTE, ///// 9
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

#endif
