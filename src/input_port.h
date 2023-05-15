#ifndef INPUT_PORT_H
#define INPUT_PORT_H

#include <stdio.h>

typedef union input_port {
	struct {
		int type;
	} port;
	struct {
		int type;
		FILE *stream;
	} stream;
	struct {
		int type;
		char *string;
	} string;
} *input_port;

char input_port_stream(FILE *stream);
char input_port_file(char *file_path);
char input_port_string(char *string);
char ip_getc(input_port ip);

#endif
