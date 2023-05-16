#include <stdlib.h>

#include "source.h"

enum type { TYPE_FILE = 1, TYPE_STRING };

char srcget(source src)
{
	char c;
	switch (src->type.type) {
	case TYPE_FILE:
		return fgetc(src->stream.stream);
	case TYPE_STRING:
		c = *src->string.string;
		if (c == '\0') {
			return EOF;
		} else {
			src->string.string++;
			return c;
		}
	default:
		exit(1);
	}
}
