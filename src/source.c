#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "sicpstd.h"
#include "source.h"

enum type { TYPE_FILE = 1, TYPE_STREAM, TYPE_STRING };

source source_part_init(void)
{
	source src;
	if (!(src = malloc(sizeof(struct source)))) {
		alloc_error("source init");
	}
	src->offset = -1;
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
	source src = source_part_init();
	src->type = TYPE_FILE;
	src->underlying.stream.stream = stream;
	return src;
}

source source_stream(FILE *stream)
{
	if (!stream) {
		eprintf("Source given null stream.");
		return NULL;
	}
	source src = source_part_init();
	src->type = TYPE_STREAM;
	src->underlying.stream.stream = stream;
	return src;
}

source source_string(char *text)
{
	if (!text) {
		eprintf("Source given null string");
		return NULL;
	}
	source src = source_part_init();
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
		c = c == EOF ? '\0' : c;
		break;
	case TYPE_STRING:
		c = *src->underlying.string.string;
		if (c) {
			src->underlying.string.string++;
		}
		break;
	default:
		inyim("'srcgetc' got an unexpected type: '%d'.", src->type);
		exit(1); // keep compiler quiet
	}
	++src->offset;
	return c;
}

int64_t source_offset(source src)
{
	return src->offset;
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
