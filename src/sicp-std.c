#include "sicp-error.h"
#include "sicp-std.h"

#include <stdlib.h>
#include <string.h>

inline void * mallocx(size_t size, const char *caller)
{
	void *ptr = malloc(size);
	if (!ptr) {
		alloc_error(caller);
	}
	return ptr;
}

inline char *strdupx(const char *s, const char *caller)
{
	char *duplicate = strdup(s);
	if (!duplicate) {
		alloc_error(caller);
	}
	return duplicate;
}
