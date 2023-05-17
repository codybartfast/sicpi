#include "string-builder.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "sicpstd.h"

string_builder new_string_builder(size_t initial_capacity)
{
	initial_capacity = initial_capacity < 1 ?
				   STRING_BUILDER_DEFAULT_INITIAL_CAPACITY :
				   initial_capacity;
	string_builder sb = malloc(sizeof(struct string_builder));
	char *buff = malloc(sizeof(char) * initial_capacity);

	if (sb == NULL || buff == NULL) {
		alloc_error("new_string_builder");
	}

	sb->buff = buff;
	sb->next = buff;
	sb->buff_end = buff + initial_capacity;

	return sb;
}

void grow(string_builder sb)
{
	char *new_buff;
	static size_t sizeLimit = __SIZE_MAX__;
	static size_t fullGrowLimit =
		__SIZE_MAX__ / STRING_BUILDER_GROWTH_FACTOR;
	size_t old_capacity = sb->buff_end - sb->buff;
	if (old_capacity == sizeLimit) {
		alloc_error("string_builder grow (limit)");
	}
	size_t new_capacity =
		old_capacity > fullGrowLimit ?
			sizeLimit :
			old_capacity * STRING_BUILDER_GROWTH_FACTOR;
	if (!(new_buff = realloc(sb->buff, new_capacity))) {
		alloc_error("string_builder grow");
	}
	char *new_next_write = new_buff + (sb->next - sb->buff);

	sb->buff = new_buff;
	sb->next = new_next_write;
	sb->buff_end = new_buff + new_capacity;
}

void sb_addc(string_builder sb, char c)
{
	if (sb->next >= sb->buff_end) {
		grow(sb);
	}
	*sb->next = c;
	++sb->next;
}

void sb_adds(string_builder sb, char *s)
{
	while (*s) {
		sb_addc(sb, *(s++));
	}
}

void terminate_string(string_builder sb)
{
	sb_addc(sb, '\0');
	--sb->next;
}

char *sb_current(string_builder sb)
{
	terminate_string(sb);
	return sb->buff;
}

char *sb_copy(string_builder sb)
{
	char *copy;
	terminate_string(sb);
	if (!(copy = strdup(sb->buff))) {
		alloc_error("sb_copy");
	}
	strcpy(copy, sb_current(sb));
	return copy;
}

void sb_clear(string_builder sb)
{
	sb->next = sb->buff;
}

void sb_free(string_builder sb)
{
	if (sb) {
		free(sb->buff);
		sb->buff = NULL;
		sb->buff_end = NULL;
		sb->next = NULL;
		free(sb);
	}
}
