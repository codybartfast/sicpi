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

static char readc(lexer lxr)
{
	return sb_addc(lxr->text, source_c(lxr->source));
}

static char peekc(lexer lxr)
{
	return source_peek(lxr->source);
}

static char add_temp(lexer lxr, char c)
{
	return sb_addc(lxr->temp, c);
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

token lexer_read(lexer lxr)
{
	skip_atmosphere(lxr);

	token tkn = malloc(sizeof(struct token));
	add_temp(lxr, readc(lxr));
	tkn->type = TKN_LIST_OPEN;
	tkn->text = sb_copy(lxr->temp);
	return tkn;
}
