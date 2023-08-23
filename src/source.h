#ifndef SOURCE_H
#define SOURCE_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#define SOURCE_EOS '\0'

/*
 * Provides a source of characters that can orignate from a file, a stream such
 * StdIn or a string.
*/
union underlying { // TODO need structs??
	struct {
		FILE * const stream;
	} stream;
	struct {
		char const * string;
	} string;
};

typedef struct source {
	const int type;
	const char *name;
	bool new_line;
	int64_t offset;
	int64_t x;
	int64_t y;
	int peeked;
	union underlying underlying;
} *source;

/*
 * Returns a pointer to new source providing characters from the given stream.
 * Returns NULL if the stream is NULL.
 */
source source_stream(FILE * const stream, char const *name);

/*
 * Attempts to open the given file and return and new source providing
 * characters from the file.
 * Returns NULL if cannot open the file.
 */
source source_file(char const *filepath);

/*
 * Returns a pointer to new source providing characters from the given string.
 * Returns NULL if the string is NULL.
 */
source source_string(char const *string, char const *name);

char const *source_name(const source src);

/*
 * Returns the next character from the source or SOURCE_EOS ('\0') if end of
 * string or stream is reached.
 */
char source_c(source src);

/*
 * The next char that will be returned by source_c
 */
char source_peek(source src);

/*
 * Zero based index of the last character returned by source_c
 */
int64_t source_offset(const source src);

/*
 * Zero based column of the last character returned by source_c
 */
int64_t source_x(const source src);

/*
 * Zero based line of the last character returned by source_c
 */
int64_t source_y(const source src);

/*
 * Releases memory allocated to the source.
 * Note if created with:
 * 	source_file:    closes the file stream,
 * 	source_stream:  does not close stream,
 * 	source_string:  does not free the string's memory.
 */
void source_close(source src);

#endif
