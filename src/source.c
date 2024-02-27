#include "source.h"

#include "sicp-error.h"
#include "sicp-std.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

source source_part_init(char const *name, enum source_type source_type,
			union underlying_data underlying_data)
{
	if (!name) {
		name = "";
	}

	size_t size = sizeof(struct source);
	struct source temp = { .name = strdupx(name,
					       "source init - duplicate name"),
			       .type = source_type,
			       .underlying_data = underlying_data };
	source src = malloc(size);
	if (!src) {
		alloc_error("source init");
	}

	memcpy(src, &temp, size);

	src->new_line = true;
	src->offset = -1;
	src->x = -1;
	src->y = -1;
	src->peeked = NO_PEEKED;
	return src;
}

source source_stream(FILE *const stream, char const *name)
{
	if (!stream) {
		eprintf("Source given null stream.");
		return NULL;
	}
	source src =
		source_part_init(name, SOURCE_TYPE_STREAM,
				 (union underlying_data){ .stream = stream });
	return src;
}

source source_file(const char *filepath)
{
	FILE *stream = fopen(filepath, "r");
	if (!stream) {
		eprintf("Failed to open file '%s'", filepath);
		perror("Error");
		return NULL;
	}
	source src =
		source_part_init(filepath, SOURCE_TYPE_FILE,
				 (union underlying_data){ .stream = stream });
	return src;
}

source source_string(char const *text, char const *name)
{
	if (!text) {
		eprintf("Source given null string");
		return NULL;
	}
	source src =
		source_part_init(name, SOURCE_TYPE_STRING,
				 (union underlying_data){ .string = text });
	return src;
}

char const *source_name(const source src)
{
	return (char const *)src->name;
}

char readc(source src)
{
	char c;
	switch (src->type) {
	case SOURCE_TYPE_FILE:
	case SOURCE_TYPE_STREAM:
		int gc = getc(src->underlying_data.stream);
		c = gc == EOF ? SOURCE_EOS : gc;
		break;
	case SOURCE_TYPE_STRING:
		c = *src->underlying_data.string;
		if (c) {
			src->underlying_data.string++;
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

char source_peek(source src)
{
	if (src->peeked == NO_PEEKED) {
		src->peeked = readc(src);
	}
	return src->peeked;
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

void source_close(source src)
{
	if (src) {
		switch (src->type) {
		case SOURCE_TYPE_FILE:
			fclose(src->underlying_data.stream);
		case SOURCE_TYPE_STREAM:
			break;
		case SOURCE_TYPE_STRING:
			break;
		default:
			inyim("source_close got an unexpected type: '%d'.",
			      src->type);
			exit(1); // keep compiler quiet
		}
		free(src);
	}
}
