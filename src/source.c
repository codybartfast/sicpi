#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "sicpstd.h"
#include "source.h"

enum type { TYPE_STREAM = 1, TYPE_STRING };

source source_file(char *filepath)
{
	FILE *file = fopen(filepath, "r");
	if (!file) {
		eprintf("Failed to open file '%s'", filepath);
		perror("Error");
		return NULL;
	}
	source src = malloc(sizeof(union source));
	src->stream.type = TYPE_STREAM;
	src->stream.stream = file;
	return src;
}

source source_string(char *text)
{
	if (!text) {
		return NULL;
	}
	source src = malloc(sizeof(union source));
	src->string.type = TYPE_STRING;
	src->string.string = text;
	return src;
}

char srcgetc(source src)
{
	char c;
	switch (src->type.type) {
	case TYPE_STREAM:
		return getc(src->stream.stream);
	case TYPE_STRING:
		c = *src->string.string;
		if (c == '\0') {
			return EOF;
		} else {
			src->string.string++;
			return c;
		}
	default:
		inyim("srcgetc got an unexpected type: '%d'.", src->type.type);
		exit(1);
	}
}

void source_close(source src)
{
	switch (src->type.type) {
	case TYPE_STREAM:
		fclose(src->stream.stream);
		break;
	case TYPE_STRING:
		break;
	default:
		inyim("source_close got an unexpected type: '%d'.",
		      src->type.type);
		exit(1);
	}
	free(src);
}
