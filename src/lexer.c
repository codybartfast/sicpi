#include "lexer.h"

#include <stdlib.h>

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

static inline lexer lexer_start_new_token(lexer lxr)
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

static inline token token_finalize(lexer lxr, token tkn, token_type type)
{
	tkn->type = type;
	tkn->text = sb_copy(lxr->text);
}

static inline char readc(lexer lxr)
{
	return sb_addc(lxr->text, source_c(lxr->source));
}

static inline char peekc(lexer lxr)
{
	return source_peek(lxr->source);
}

static inline char add_temp(lexer lxr, char c)
{
	return sb_addc(lxr->temp, c);
}

static inline bool is_delimiter_or_eos(char c)
{
	return is_delimiter(c) || c == SOURCE_EOS;
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

static inline bool read_identifier(lexer lxr)
{
	while (is_subsequent(peekc(lxr))) {
		add_temp(lxr, readc(lxr));
	}
	if (is_delimiter_or_eos(peekc(lxr))) {
		return true;
	} else {
		// lxr->msg = invalid char in identifier
		return false;
	}
}

token lexer_read(lexer lxr)
{
	skip_atmosphere(lxr);
	token_type type = TKN_UNDEFINED;

	char c = readc(lxr);
	lexer_start_new_token(lxr);
	token tkn = token_new(lxr);
	add_temp(lxr, c);
// printf("one: %c\n", c);
	switch (c) {
	case '(':
		type = TKN_LIST_OPEN;
		break;
	default:
// printf("two\n");
		if (is_initial(c)) {
// printf("three\n");
			type = TKN_IDENTIFIER;
			read_identifier(lxr);
			break;
		}
	}

	tkn->type = type;
	tkn->text = sb_copy(lxr->temp);
	return tkn;
}
