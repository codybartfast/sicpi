#include "token.h"

inline enum token_type tkn_type(token tkn)
{
	return tkn->type;
}

inline char *tkn_text(token tkn)
{
	return tkn->text;
}

inline char *tkn_err_msg(token tkn)
{
	return tkn->err_msg;
}

inline long tkn_offset(token tkn)
{
	return tkn->offset;
}

inline long tkn_x(token tkn)
{
	return tkn->x;
}

inline long tkn_y(token tkn)
{
	return tkn->y;
}
