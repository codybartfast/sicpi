#include "token.h"

/*
 * Note: meta_data is not freed.  In general token doesn't know the nature of
 * its meta data and specifically (at time of writing) if the meta_data is a
 * 'source' then there will be multiple tokens referencing the same source.
 */

inline void token_free(token tkn)
{
	if (tkn) {
		free(tkn->text);
		tkn->text = NULL;

		free(tkn);
	}
}

inline enum token_type token_type(token tkn)
{
	return tkn->type;
}

inline char *token_text(token tkn)
{
	return tkn->text;
}

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

inline void *token_metadata(token tkn)
{
	return tkn->meta_data;
}
