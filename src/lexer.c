#include "lexer.h"

#include <stdlib.h>

#include "token.h"
#include "sicpstd.h"
#include "source.h"

lexer lexer_new(source src)
{
	if (!src) {
		return NULL;
	}
	lexer lxr = malloc(sizeof(struct lexer));
	if (!lxr) {
		alloc_error("lexor_new");
	}
	lxr->source = src;
	lxr->sb = new_string_builder(0);
	return lxr;
}

void lexer_free(lexer lxr)
{
	if (lxr) {
		sb_free(lxr->sb);
		lxr->source = NULL;
		lxr->sb = NULL;
		free(lxr);
	}
}
