#include "token.h"

token_type tkn_type(token tkn)
{
	return tkn->type;
}

char *tkn_text(token tkn)
{
	return tkn->text;
}
