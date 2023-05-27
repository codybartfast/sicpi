#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sicpstd.h"
#include "source.h"

#define NO_PEEKED -1

enum type { TYPE_FILE = 1, TYPE_STREAM, TYPE_STRING };

source source_part_init(char *name)
{
	if (!name) {
		name = "";
	}
	source src = malloc(sizeof(struct source));
	if (!src) {
		alloc_error("source init");
	}
	char *dup_name = strdup(name);
	if (!dup_name) {
		alloc_error("source init - duplicate name");
	}
	src->name = dup_name;
	src->new_line = true;
	src->offset = -1;
	src->x = -1;
	src->y = -1;
	src->peeked = NO_PEEKED;
	return src;
}

source source_stream(FILE *stream, char *name)
{
	if (!stream) {
		eprintf("Source given null stream.");
		return NULL;
	}
	source src = source_part_init(name);
	src->type = TYPE_STREAM;
	src->underlying.stream.stream = stream;
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
	source src = source_part_init(filepath);
	src->type = TYPE_FILE;
	src->underlying.stream.stream = stream;
	return src;
}

source source_string(char *text, char *name)
{
	if (!text) {
		eprintf("Source given null string");
		return NULL;
	}
	source src = source_part_init(name);
	src->type = TYPE_STRING;
	src->underlying.string.string = text;
	return src;
}

char *source_name(source src)
{
	return src->name;
}

char readc(source src)
{
	char c;
	switch (src->type) {
	case TYPE_FILE:
	case TYPE_STREAM:
		c = getc(src->underlying.stream.stream);
		c = c == EOF ? SOURCE_EOS : c;
		break;
	case TYPE_STRING:
		c = *src->underlying.string.string;
		if (c) {
			src->underlying.string.string++;
		}
		break;
	default:
		inyim("'source_c' got an unexpected type: '%d'.", src->type);
		exit(1); // keep compiler quiet
	}
	return c;
}

char source_c(source src)
{
	char c;

	if (src->peeked == NO_PEEKED) {
		c = readc(src);
	} else {
		c = (char)src->peeked;
		src->peeked = NO_PEEKED;
	}

	if (c) {
		if (src->offset == INT64_MAX) {
			eprintfx("source: exceeded max size.");
		}
		src->offset++;
		if (src->new_line) {
			src->new_line = false;
			src->x = -1;
			src->y++;
		}
		src->x++;
		if (c == '\n') {
			src->new_line = true;
		}
	}
	return c;
}

int64_t source_offset(source src)
{
	return src->offset;
}

int64_t source_x(source src)
{
	return src->x;
}

int64_t source_y(source src)
{
	return src->y;
}

char source_peek(source src)
{
	if (src->peeked == NO_PEEKED) {
		src->peeked = readc(src);
	}
	return src->peeked;
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
