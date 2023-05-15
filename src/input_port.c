#include <stdlib.h>

#include "input_port.h"

enum type { KIND_FILE = 1, KIND_STRING };

char ip_getc(input_port ip)
{
	char c;
	switch (ip->port.type) {
	case KIND_FILE:
		return fgetc(ip->stream.stream);
	case KIND_STRING:
		c = *ip->string.string;
		if (c == '\0') {
			return EOF;
		} else {
			ip->string.string++;
			return c;
		}
	default:
		exit(1);
	}
}
