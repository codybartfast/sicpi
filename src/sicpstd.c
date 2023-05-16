#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void eprintf(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	fprintf(stderr, "Error: ");
	vfprintf(stderr, msg, args);
	fprintf(stderr, "\n");
	va_end(args);
}

void inyim(char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	fprintf(stderr, "***  INTERNAL  ERROR  ***\n");
	fprintf(stderr, " (It's not you, it's me)\n");
	fprintf(stderr, "Error: ");
	vfprintf(stderr, msg, args);
	fprintf(stderr, "\n");
	va_end(args);
}
