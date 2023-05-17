#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "sicpstd.h"
#include "source.h"

enum type { TYPE_STREAM = 1, TYPE_STRING };

source source_stream(FILE *stream)
{
	source src;
	if (!stream) {
		eprintf("Source given null stream.");
		return NULL;
	}
	if (!(src = malloc(sizeof(union source)))) {
		alloc_error("source_stream");
	}
	src->stream.type = TYPE_STREAM;
	src->stream.stream = stream;
	return src;
}

source source_file(char *filepath)
{
	FILE *stream = fopen(filepath, "r");
	if (!stream) {
		eprintf("Failed to open file '%s'", filepath);
		perror("Error");
		return NULL;
	}
	return source_stream(stream);
}

source source_string(char *text)
{
	source src;
	if (!text) {
		eprintf("Source given null string");
		return NULL;
	}
	if (!(src = malloc(sizeof(union source)))) {
		alloc_error("source_string");
	}
	src->string.type = TYPE_STRING;
	src->string.string = text;
	return src;
}

char srcgetc(source src)
{
	char c;
	switch (src->type.type) {
	case TYPE_STREAM:
		c = getc(src->stream.stream);
		return c == EOF ? '\0' : c;
	case TYPE_STRING:
		c = *src->string.string;
		if (c) {
			src->string.string++;
		}
		return c;
	default:
		inyim("'srcgetc' got an unexpected type: '%d'.", src->type.type);
		exit(1); // keep compiler quiet
	}
}

void source_close(source src)
{
	if (src) {
		switch (src->type.type) {
		case TYPE_STREAM:
			fclose(src->stream.stream);
			break;
		case TYPE_STRING:
			break;
		default:
			inyim("source_close got an unexpected type: '%d'.",
			      src->type.type);
			exit(1); // keep compiler quiet
		}
		free(src);
	}
}
