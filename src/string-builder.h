#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct string_builder {
	char *buff;
	char *next;
	char *buff_end;
} *string_builder;

string_builder new_string_builder(size_t initial_capacity);
void sb_addc(string_builder sb, char c);
void sb_adds(string_builder sb, char *s);
char *sb_current(string_builder sb);
char *sb_copy(string_builder sb);
void sb_clear(string_builder sb);
void sb_free(string_builder sb);

#endif