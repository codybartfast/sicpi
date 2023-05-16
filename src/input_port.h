#ifndef SOURCE_H
#define SOURCE_H

#include <stdio.h>

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

char source_stream(FILE *stream);
char source_file(char *file_path);
char source_string(char *string);
char srcget(source src);

#endif
