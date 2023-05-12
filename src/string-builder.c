#include "string-builder.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_INITIAL_CAPACITY 32
#define ALLOC_FAIL_MSG                                                         \
	"ERROR: Failed to allocate memory for string builder (out of memory)"

void alloc_error(void)
{
	fputs(ALLOC_FAIL_MSG, stderr);
	exit(1);
}

string_builder new_string_builder(size_t initial_capacity)
{
	initial_capacity = initial_capacity < 1 ? DEFAULT_INITIAL_CAPACITY :
						  initial_capacity;
	string_builder sb = malloc(sizeof(struct string_builder));
	char *buff = malloc(sizeof(char) * initial_capacity);

	if (sb == NULL || buff == NULL) {
		alloc_error();
	}

	sb->buff = buff;
	sb->next = buff;
	sb->buff_end = buff + initial_capacity;

	return sb;
}

void grow(string_builder sb)
{
	static size_t sizeLimit = __SIZE_MAX__;
	static size_t halfSizeLimit = __SIZE_MAX__ / 2;
	size_t old_capacity = sb->buff_end - sb->buff;
	if (old_capacity == sizeLimit) {
		alloc_error();
	}
	size_t new_capacity =
		old_capacity > halfSizeLimit ? sizeLimit : old_capacity * 2;

	char *new_buff = realloc(sb->buff, new_capacity);
	if (new_buff == NULL) {
		alloc_error();
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
	terminate_string(sb);
	char *copy = strdup(sb->buff);
	if (copy == NULL) {
		alloc_error();
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
	free(sb->buff);
	sb->buff = NULL;
	sb->buff_end = NULL;
	sb->next = NULL;
	free(sb);
}
