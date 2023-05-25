#include "lexer.h"

#include <stdlib.h>

#include "token.h"
#include "sicpstd.h"
#include "source.h"

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

static char skip_atmosphere(lexer lxr)
{
}

token lexer_read(lexer lxr)
{
	// char c = skip_atmosphere(lxr);
	return NULL;
}
