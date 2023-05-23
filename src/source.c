#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "sicpstd.h"
#include "source.h"

enum type { TYPE_FILE = 1, TYPE_STREAM, TYPE_STRING };

source source_file(char *filepath)
{
	source src;
	FILE *stream = fopen(filepath, "r");
	if (!stream) {
		eprintf("Failed to open file '%s'", filepath);
		perror("Error");
		return NULL;
	}
	if (!(src = malloc(sizeof(struct source)))) {
		alloc_error("source_file");
	}
	src->type = TYPE_FILE;
	src->underlying.stream.stream = stream;
	return src;
}

source source_stream(FILE *stream)
{
	source src;
	if (!stream) {
		eprintf("Source given null stream.");
		return NULL;
	}
	if (!(src = malloc(sizeof(struct source)))) {
		alloc_error("source_stream");
	}
	src->type = TYPE_STREAM;
	src->underlying.stream.stream = stream;
	return src;
}

source source_string(char *text)
{
	source src;
	if (!text) {
		eprintf("Source given null string");
		return NULL;
	}
	if (!(src = malloc(sizeof(struct source)))) {
		alloc_error("source_string");
	}
	src->type = TYPE_STRING;
	src->underlying.string.string = text;
	return src;
}

char srcgetc(source src)
{
	char c;
	switch (src->type) {
	case TYPE_FILE:
	case TYPE_STREAM:
		c = getc(src->underlying.stream.stream);
		return c == EOF ? '\0' : c;
	case TYPE_STRING:
		c = *src->underlying.string.string;
		if (c) {
			src->underlying.string.string++;
		}
		return c;
	default:
		inyim("'srcgetc' got an unexpected type: '%d'.",
		      src->type);
		exit(1); // keep compiler quiet
	}
}

void source_close(source src)
{
	if (src) {
		switch (src->type) {
		case TYPE_FILE:
			fclose(src->underlying.stream.stream);
		case TYPE_STREAM:
			break;
		case TYPE_STRING:
			break;
		default:
			inyim("source_close got an unexpected type: '%d'.",
			      src->type);
			exit(1); // keep compiler quiet
		}
		free(src);
	}
}
