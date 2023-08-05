#include "token.h"

inline enum token_type token_type(token tkn)
{
	return tkn->type;
}

inline char *token_text(token tkn)
{
	return tkn->text;
}

// inline char *token_error_message(token tkn)
// {
// 	return tkn->err_msg;
// }

inline long token_offset(token tkn)
{
	return tkn->offset;
}

inline long token_x(token tkn)
{
	return tkn->x;
}

inline long token_y(token tkn)
{
	return tkn->y;
}
