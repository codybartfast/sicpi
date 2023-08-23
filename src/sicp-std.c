#include "sicp-error.h"
#include "sicp-std.h"

#include <string.h>

inline char *strdupx(const char *s, const char *caller)
{
	char *duplicate = strdup(s);
	if (!duplicate) {
		alloc_error(caller);
	}
	return duplicate;
}
