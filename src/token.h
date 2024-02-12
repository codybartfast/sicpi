#ifndef TOKEN_H
#define TOKEN_H

#include "string-builder.h"

#include <inttypes.h>

enum token_type {
	TOKEN_ERROR = -2,
	TOKEN_EOS = -1,
	TOKEN_UNSPECIFIED = 0,
	TOKEN_IDENTIFIER, //////  1
	TOKEN_LIST_OPEN, ///////  2
	TOKEN_LIST_CLOSE, //////  3
	TOKEN_NUMBER_INTEGER, //  4
	TOKEN_NUMBER_DECIMAL, //  5
	TOKEN_STRING, //////////  6
	TOKEN_DOT, /////////////  7
	TOKEN_QUOTE, ///////////  8
	TOKEN_QUASIQUOTE, //////  9
	TOKEN_UNQUOTE, ///////// 10
};

typedef struct token {
	void *meta_data;
	enum token_type type;
	char *text;
	int64_t offset;
	int64_t x; // zero-based column
	int64_t y; // zero-based line

	// I don't like including this here as I would prefer 'token' were
	// ignorant of the token table, but this seems less ugly than anything
	// else I can think of (like having the parser populate the token table)
	int64_t key;
} *token;

enum token_type token_type(token tkn);
char *token_text(token tkn);
// char *token_error_message(token tkn);
int64_t token_offset(token tkn);
int64_t token_x(token tkn);
int64_t token_y(token tkn);
void *token_metadata(token tkn);
uint32_t token_key(token tkn);
void token_free(token tkn);

typedef struct token_source {
	void *state;
	token (*read_token)(void *state);
} *token_source;

// Todo: use or remove.  Purpose?  (possibly intended to prevent a 'randomly'
//       instantiated object being passed around)
void token_source_init(token_source tkn_src);

token token_read(token_source tkn_src);

#endif
