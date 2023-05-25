#include "sicpstd.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void alloc_error(char *caller)
{
	fprintf(stderr,
		"ERROR: '%s' failed to allocate memory (out of memory?).",
		caller);
	exit(1);
}

void eprintf(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	fprintf(stderr, "Error: ");
	vfprintf(stderr, msg, args);
	fprintf(stderr, "\n");
	va_end(args);
}

void eprintfx(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	fprintf(stderr, "Error: ");
	vfprintf(stderr, msg, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(1);
}

void inyim(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	fprintf(stderr, "***  INTERNAL  ERROR  ***\n");
	fprintf(stderr, " (It's not you, it's me)\n");
	fprintf(stderr, "Error: ");
	vfprintf(stderr, msg, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(1);
}
