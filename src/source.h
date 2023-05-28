#ifndef SOURCE_H
#define SOURCE_H

#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>

#define SOURCE_EOS '\0'

/*
 * Provides a source of characters that can orignate from a file, a stream such
 * StdIn or a string.
*/
typedef struct source {
	int type;
	char *name;
	bool new_line;
	int64_t offset;
	int64_t x;
	int64_t y;
	int peeked;
	union {
		struct {
			FILE *stream;
		} stream;
		struct {
			char *string;
		} string;
	} underlying;
} *source;

/*
 * Returns a pointer to new source providing characters from the given stream.
 * Returns NULL if the stream is NULL.
 */
source source_stream(FILE *stream, char *name);

/*
 * Attempts to open the given file and return and new source providing
 * characters from the file.
 * Returns NULL if cannot open the file.
 */
source source_file(char *filepath);

/*
 * Returns a pointer to new source providing characters from the given string.
 * Returns NULL if the string is NULL.
 */
source source_string(char *string, char *name);

char *source_name(source src);

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
int64_t source_offset(source src);

/*
 * Zero based column of the last character returned by source_c
 */
int64_t source_x(source src);

/*
 * Zero based line of the last character returned by source_c
 */
int64_t source_y(source src);

/*
 * Releases memory allocated to the source.
 * Note if created with:
 * 	source_file:    closes the file stream,
 * 	source_stream:  does not close stream,
 * 	source_string:  does not free the string's memory.
 */
void source_close(source src);

#endif
