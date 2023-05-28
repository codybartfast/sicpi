#include "lexer.h"

#include <stdlib.h>
#include <string.h>

#include "character-classes.h"
#include "sicpstd.h"
#include "source.h"
#include "token.h"

lexer lexer_new(source src)
{
	if (!src) {
		eprintf("lexer_new given null source");
		return NULL;
	}
	lexer lxr = malloc(sizeof(struct lexer));
	if (!lxr) {
		alloc_error("lexor_new");
	}
	lxr->source = src;
	lxr->text = sb_new(0);
	lxr->temp = sb_new(0);
	lxr->is_errored = false;
	return lxr;
}

void lexer_free(lexer lxr)
{
	if (lxr) {
		sb_free(lxr->text);
		sb_free(lxr->temp);
		lxr->source = NULL;
		lxr->text = NULL;
		lxr->temp = NULL;
		free(lxr);
	}
}

inline bool lexer_is_errored(lexer lxr)
{
	return lxr->is_errored;
}

static inline void lexer_start_new_token(lexer lxr)
{
	sb_clear(lxr->temp);
}

static inline token token_new(lexer lxr)
{
	token tkn = malloc(sizeof(struct token));
	if (!tkn) {
		alloc_error("token_new");
	}
	tkn->lxr = lxr;
	tkn->type = TKN_UNDEFINED;
	tkn->text = NULL;
	tkn->err_msg = NULL;
	tkn->offset = source_offset(lxr->source);
	tkn->x = source_x(lxr->source);
	tkn->y = source_y(lxr->source);
	return tkn;
}

// static inline token token_finalize(lexer lxr, token tkn, token_type type)
// {
// 	tkn->type = type;
// 	tkn->text = sb_copy(lxr->text);
// }

static inline char readc(lexer lxr)
{
	return sb_addc(lxr->text, source_c(lxr->source));
}

static inline char peekc(lexer lxr)
{
	return source_peek(lxr->source);
}

static inline char temp_addc(lexer lxr, char c)
{
	return sb_addc(lxr->temp, c);
}

static inline char temp_readc(lexer lxr)
{
	return sb_addc(lxr->temp, readc(lxr));
}

// static inline bool is_delimiter_or_eos(char c)
// {
// 	return is_delimiter(c) || c == SOURCE_EOS;
// }

static inline bool skip_whitespace(lexer lxr)
{
	bool did_skip = false;
	while (is_whitespace(peekc(lxr))) {
		did_skip = true;
		readc(lxr);
	}
	return did_skip;
}

static inline bool skip_comment(lexer lxr)
{
	bool did_skip = false;
	if (peekc(lxr) == ';') {
		did_skip = true;
		readc(lxr);
		char c;
		while (((c = peekc(lxr)) != '\n' && (c != SOURCE_EOS))) {
			readc(lxr);
		}
	}
	return did_skip;
}

static inline void skip_atmosphere(lexer lxr)
{
	while (skip_whitespace(lxr) || skip_comment(lxr))
		;
}

static char *str_identifier = "identifier";
static char *str_number = "number";
static char *str_in = "in";
static char *str_ending = "at end of";

static char *error_message(char c, char *relation, char *type, lexer lxr)
{
	char err_buff[256];
	sprintf(err_buff, "Unexpected character '%c', 0x%0X, %s %s: '%.128s'.",
		c, c, relation, type, sb_current(lxr->temp));
	return strdup(err_buff);
}

static inline char *check_at_end_of_token(lexer lxr, char *tkn_type_str)
{
	char c = peekc(lxr);
	if (is_delimiter(c) || c == SOURCE_EOS) {
		return NULL;
	} else {
		char c = temp_readc(lxr);
		return error_message(c, str_in, tkn_type_str, lxr);
	}
}

static inline char *read_identifier(lexer lxr)
{
	while (is_subsequent(peekc(lxr))) {
		temp_readc(lxr);
	}
	return check_at_end_of_token(lxr, str_identifier);
}

static inline char *read_decimal_part(lexer lxr)
{
	if (!is_digit(peekc(lxr))) {
		return error_message('.', str_ending, str_number, lxr);
	}
	while (is_digit(peekc(lxr))) {
		temp_readc(lxr);
	}
	return check_at_end_of_token(lxr, str_number);
}

static inline char *read_number(lexer lxr)
{
	while (is_digit(peekc(lxr))) {
		temp_readc(lxr);
	}
	if (peekc(lxr) == '.') {
		temp_readc(lxr);
		return read_decimal_part(lxr);
	}
	return check_at_end_of_token(lxr, str_number);
}

token lexer_read(lexer lxr)
{
	skip_atmosphere(lxr);
	token_type type = TKN_UNDEFINED;
	char *err_msg = NULL;

	char c = readc(lxr);
	lexer_start_new_token(lxr);
	token tkn = token_new(lxr);
	temp_addc(lxr, c);
	switch (c) {
	case '(':
		type = TKN_LIST_OPEN;
		break;
	case ')':
		type = TKN_LIST_CLOSE;
		break;
	default:
		if (is_initial(c)) {
			type = TKN_IDENTIFIER;
			err_msg = read_identifier(lxr);
		} else if (is_digit(c)) {
			type = TKN_NUMBER;
			err_msg = read_number(lxr);
		}
		break;
	}

	tkn->text = sb_copy(lxr->temp);
	if (err_msg) {
		tkn->err_msg = err_msg;
		tkn->type = TKN_ERROR;
		lxr->is_errored = true;
	} else {
		tkn->type = type;
	}
	return tkn;
}
