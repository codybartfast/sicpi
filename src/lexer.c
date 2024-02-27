#include "lexer.h"

#include "character-classes.h"
#include "sicp-error.h"
#include "sicp-std.h"
#include "source.h"
#include "token.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

char err_buff[256];

lexer lexer_new(source src)
{
	/*
	 * Code below may rely on SOUCE_EOS being zero, e.g., for string
	 * termination or flow control.
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
	lxr->error_message = NULL;
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
		lxr->text = NULL;

		sb_free(lxr->temp);
		lxr->temp = NULL;

		free(lxr->error_message);
		lxr->error_message = NULL;

		free(lxr);
	}
}

inline bool lexer_is_errored(lexer lxr)
{
	return lxr->is_errored;
}

inline char *lexer_error_message(lexer lxr)
{
	return lxr->error_message;
}

static inline token token_new(lexer lxr)
{
	token tkn = malloc(sizeof(struct token));
	if (!tkn) {
		alloc_error("token_new");
	}
	tkn->type = TOKEN_UNSPECIFIED;
	tkn->text = NULL;
	tkn->offset = source_offset(lxr->source);
	tkn->x = source_x(lxr->source);
	tkn->y = source_y(lxr->source);
	tkn->meta_data = lxr->source;
	tkn->key = 0;
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
	return strdupx(err_buff, "lexer:error_message");
}

static char *error_unexpected_end_of_string(lexer lxr)
{
	sprintf(err_buff,
		"No closing double quote '\"' found for string starting: '%.128s'.",
		sb_current(lxr->temp));
	return strdupx(err_buff, "lexer:error_unexpected_end_of_string");
}

static char *error_expected_expression(char prefix)
{
	sprintf(err_buff, "Expected an expression after '%c'.", prefix);
	return strdupx(err_buff, "error_expected_expression");
}

static token handle_lexer_is_errored(lexer lxr)
{
	token tkn = token_new(lxr);
	tkn->text = NULL;
	// lxr->error_message =
	// 	strdupx("Attempted to read lexer after an earlier error.");
	tkn->type = TOKEN_ERROR;
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

static inline char *read_number(lexer lxr, enum token_type *type)
{
	while (is_digit(peekc(lxr))) {
		temp_add_readc(lxr);
	}
	if (peekc(lxr) == '.') {
		*type = TOKEN_NUMBER_DECIMAL;
		temp_add_readc(lxr);
		return read_decimal_part(lxr);
	}
	*type = TOKEN_NUMBER_INTEGER;
	return check_at_end_of_token(lxr, "number");
}

static inline char *read_string(lexer lxr)
{
	char c;
	while ((c = readc(lxr)) != SOURCE_EOS) {
		char next; // Emscripten won't allow declaration to follow label
		switch (c) {
		case '"':
			return check_at_end_of_token(lxr, "string");
		case '\\':
			next = readc(lxr);
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
		return handle_lexer_is_errored(lxr);
	}

	skip_atmosphere(lxr);

	char *err_msg = NULL;
	enum token_type type = TOKEN_UNSPECIFIED;
	sb_clear(lxr->temp);

	char c = readc(lxr);
	token tkn = token_new(lxr);
	char next = peekc(lxr);

	switch (c) {
	case '(':
		temp_addc(lxr, c);
		type = TOKEN_LIST_OPEN;
		break;
	case ')':
		temp_addc(lxr, c);
		type = TOKEN_LIST_CLOSE;
		break;
	case '+':
	case '-':
		temp_addc(lxr, c);
		if (is_digit(next) || next == '.') {
			err_msg = read_number(lxr, &type);
		} else {
			type = TOKEN_IDENTIFIER;
			err_msg = check_at_end_of_token(lxr, "+/-");
		}
		break;
	case '.':
		temp_addc(lxr, c);
		if (is_digit(next)) {
			type = TOKEN_NUMBER_DECIMAL;
			err_msg = read_decimal_part(lxr);
		} else {
			type = TOKEN_DOT;
			err_msg = check_at_end_of_token(lxr, "dot");
		}
		break;
	case '"':
		type = TOKEN_STRING;
		err_msg = read_string(lxr);
		break;
	case '\'':
		type = TOKEN_QUOTE;
		err_msg = read_expression_prefix(lxr, c, next, "quote");
		break;
	case '`':
		type = TOKEN_QUASIQUOTE;
		err_msg = read_expression_prefix(lxr, c, next, "quasiquote");
		break;
	case ',':
		type = TOKEN_UNQUOTE;
		err_msg = read_expression_prefix(lxr, c, next, "unquote");
		break;
	case SOURCE_EOS:
		type = TOKEN_EOS;
		break;
	default:
		temp_addc(lxr, c);
		if (is_initial(c)) {
			type = TOKEN_IDENTIFIER;
			err_msg = read_identifier(lxr);
		} else if (is_digit(c)) {
			err_msg = read_number(lxr, &type);
		}
		break;
	}

	tkn->text = sb_copy(lxr->temp);

	if (type == TOKEN_UNSPECIFIED) {
		err_msg = error_message(c, "at start of", "token", lxr);
	}

	if (err_msg) {
		lxr->error_message = err_msg;
		tkn->type = TOKEN_ERROR;
		lxr->is_errored = true;
	} else {
		tkn->type = type;
	}
	return tkn;
}

void lexer_set_token_source(token_source tkn_src, lexer lxr)
{
	tkn_src->read_token = (token(*)(void *state))lexer_read;
	tkn_src->state = lxr;
}



struct source no_meta_data_source = {
	.type = SOURCE_TYPE_STRING,
	.name = "No meta data source",
	.new_line = true,
	.offset = 0,
	.x = 0,
	.y = 0,
	.peeked = NO_PEEKED,
	.underlying_data = { .string = "\"No meta data\""},
};

struct token no_meta_data_token = {
	.meta_data = &no_meta_data_source,
	.type = TOKEN_STRING,
	.text = "No meta data",
	.offset = 1,
	.x = 1,
	.y = 0,
	.key = 0,
};
