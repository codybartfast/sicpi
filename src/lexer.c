#include "lexer.h"

#include "character-classes.h"
#include "sicp-error.h"
#include "source.h"
#include "token.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

char err_buff[256];

lexer lexer_new(source src)
{
	/*
	 * There's at least one place where lexer relies on SOURCE_EOS being
	 * implemented as 0.
	 */
	assert(SOURCE_EOS == '\0');

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

void lexer_free_source(lexer lxr)
{
	if (lxr && lxr->source) {
		source_close(lxr->source);
		lxr->source = NULL;
	}
}

void lexer_free(lexer lxr)
{
	if (lxr) {
		sb_free(lxr->text);
		sb_free(lxr->temp);
		lxr->text = NULL;
		lxr->temp = NULL;
		free(lxr);
	}
}

inline bool lexer_is_errored(lexer lxr)
{
	return lxr->is_errored;
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

static inline char temp_add_readc(lexer lxr)
{
	return sb_addc(lxr->temp, readc(lxr));
}

static char *error_message(char c, char *relation, char *type, lexer lxr)
{
	sprintf(err_buff, "Unexpected character '%c', 0x%0X, %s %s: '%.128s'.",
		c, c, relation, type, sb_current(lxr->temp));
	return strdup(err_buff);
}

static char *error_unexpected_end_of_string(lexer lxr)
{
	sprintf(err_buff,
		"No closing double quote '\"' found for string starting: '%.128s'.",
		sb_current(lxr->temp));
	return strdup(err_buff);
}

static char *error_expected_expression(char prefix)
{
	sprintf(err_buff, "Expected an expression after '%c'.", prefix);
	return strdup(err_buff);
}

static token lexer_is_errored_token(lexer lxr)
{
	token tkn = token_new(lxr);
	tkn->text = strdup("");
	tkn->err_msg = strdup(
		"Attempted to read from a lexer after a preceeding error.");
	tkn->type = TKN_ERROR;
	return tkn;
}

static inline char *check_at_end_of_token(lexer lxr, char *tkn_type_str)
{
	char c = peekc(lxr);
	if (is_delimiter(c) || c == SOURCE_EOS) {
		return NULL;
	} else {
		char c = temp_add_readc(lxr);
		return error_message(c, "in", tkn_type_str, lxr);
	}
}

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

static inline char *read_identifier(lexer lxr)
{
	while (is_subsequent(peekc(lxr))) {
		temp_add_readc(lxr);
	}
	return check_at_end_of_token(lxr, "identifier");
}

static inline char *read_decimal_part(lexer lxr)
{
	if (!is_digit(peekc(lxr))) {
		return error_message('.', "at end of", "number", lxr);
	}
	while (is_digit(peekc(lxr))) {
		temp_add_readc(lxr);
	}
	return check_at_end_of_token(lxr, "number");
}

static inline char *read_number(lexer lxr)
{
	while (is_digit(peekc(lxr))) {
		temp_add_readc(lxr);
	}
	if (peekc(lxr) == '.') {
		temp_add_readc(lxr);
		return read_decimal_part(lxr);
	}
	return check_at_end_of_token(lxr, "number");
}

static inline char *read_string(lexer lxr)
{
	char c;
	while ((c = readc(lxr)) != SOURCE_EOS) {
		switch (c) {
		case '"':
			return check_at_end_of_token(lxr, "string");
		case '\\':
			char next = readc(lxr);
			switch (next) {
			case '\\':
			case '"':
				temp_addc(lxr, next);
				break;
			default:
				return error_message(
					next, "following \\ (backslash) in",
					"string", lxr);
			}
			break;
		case SOURCE_EOS:

		default:
			temp_addc(lxr, c);
			break;
		}
	}
	return error_unexpected_end_of_string(lxr);
}

static inline char *read_expression_prefix(lexer lxr, char c, char next,
					   char *prefixName)
{
	temp_addc(lxr, c);

	if (is_whitespace(next)) {
		return error_message(next, "immediatedly after", prefixName,
				     lxr);
	} else if (next == SOURCE_EOS) {
		return error_expected_expression(c);
	}
	return NULL;
}

token lexer_read(lexer lxr)
{
	if (lexer_is_errored(lxr)) {
		return lexer_is_errored_token(lxr);
	}

	skip_atmosphere(lxr);

	char *err_msg = NULL;
	token_type type = TKN_UNDEFINED;
	sb_clear(lxr->temp);

	char c = readc(lxr);
	token tkn = token_new(lxr);
	char next = peekc(lxr);

	switch (c) {
	case '(':
		temp_addc(lxr, c);
		type = TKN_LIST_OPEN;
		break;
	case ')':
		temp_addc(lxr, c);
		type = TKN_LIST_CLOSE;
		break;
	case '+':
	case '-':
		temp_addc(lxr, c);
		if (is_digit(next) || next == '.') {
			type = TKN_NUMBER;
			err_msg = read_number(lxr);
		} else {
			type = TKN_IDENTIFIER;
			err_msg = check_at_end_of_token(lxr, "+/-");
		}
		break;
	case '.':
		temp_addc(lxr, c);
		if (is_digit(next)) {
			type = TKN_NUMBER;
			err_msg = read_decimal_part(lxr);
		} else {
			type = TKN_DOT;
			err_msg = check_at_end_of_token(lxr, "dot");
		}
		break;
	case '"':
		type = TKN_STRING;
		err_msg = read_string(lxr);
		break;
	case '\'':
		type = TKN_QUOTE;
		err_msg = read_expression_prefix(lxr, c, next, "quote");
		break;
	case '`':
		type = TKN_QUASIQUOTE;
		err_msg = read_expression_prefix(lxr, c, next, "quasiquote");
		break;
	case ',':
		type = TKN_UNQUOTE;
		err_msg = read_expression_prefix(lxr, c, next, "unquote");
		break;
	case SOURCE_EOS:
		type = TKN_EOF;
		break;
	default:
		temp_addc(lxr, c);
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

	if (type == TKN_UNDEFINED) {
		err_msg = error_message(c, "at start of", "token", lxr);
	}

	if (err_msg) {
		tkn->err_msg = err_msg;
		tkn->type = TKN_ERROR;
		lxr->is_errored = true;
	} else {
		tkn->type = type;
	}
	return tkn;
}

void lexer_set_token_source(lexer lxr, token_source *tkn_src){
	tkn_src->read_token = (token (*)(void *state))lexer_read;
	tkn_src->state = lxr;
}
