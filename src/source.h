#ifndef SOURCE_H
#define SOURCE_H

#include <stdio.h>

/*
 * Provides a source of characters that can orignate from a file, a stream such
 * StdIn or a string.
*/
typedef union source {
	struct {
		int type;
	} type;
	struct {
		int type;
		FILE *stream;
	} stream;
	struct {
		int type;
		char *string;
	} string;
} *source;

/*
 * Returns a pointer to new source providing characters from the given stream.
 * Returns NULL if the stream is NULL.
 */
source source_stream(FILE *stream);

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
source source_string(char *string);

/*
 * Returns the next character from the source or '\0' if end of string or stream
 * is reached
 */
char srcgetc(source src);

/*
 * Releases memory allocated to the source.
 * Note if created with:
 * 	source_file:    closes the file stream,
 * 	source_stream:  does not close stream,
 * 	source_string:  does not free the string's memory.
 */
void source_close(source src);

#endif