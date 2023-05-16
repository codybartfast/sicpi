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

source source_stream(FILE *stream);
source source_file(char *filepath);
source source_string(char *string);
char srcgetc(source src);
void source_close(source src);

#endif
