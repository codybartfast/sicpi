#ifndef PARSER_H
#define PARSER_H

#include "object.h"
#include "token.h"

#include <stdbool.h>

typedef struct parser {
	token_source token_source;
	bool is_errored;
	char *error_message;
} *parser;

void parser_init(parser parser);

object parse(parser parser);

#endif
