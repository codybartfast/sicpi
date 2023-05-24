#include "lexer.h"

#include <stdlib.h>

#include "token.h"
#include "source.h"

lexer lexer_new(source src)
{
	if (!src) {
		return NULL;
	}
	lexer lxr = malloc(sizeof(struct lexer));
	return lxr;
}
