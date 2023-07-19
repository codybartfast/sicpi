#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stdbool.h>
#include <stdlib.h>

/*
 * Current state of a builder.
 * Members are not inteded for use by the consumer.
 */
typedef struct string_builder {
	char *buff;
	char *next;
	char *buff_end;
} *string_builder;

#define STRING_BUILDER_DEFAULT_INITIAL_CAPACITY 32

#define STRING_BUILDER_GROWTH_FACTOR 2

/*
 * Returns a pointer to a new string_builder.
 * If initial_capacity is zero then the default initial capacity is used.
*/
string_builder sb_new(size_t initial_capacity);

/*
 * Adds the character to the string_builder, and returns the same character.
*/
char sb_addc(string_builder sb, char c);

/*
 * Copies the string into the string_builder
*/
char *sb_adds(string_builder sb, char *str);

/*
 * Returns a reference to the string_builder's internal buffer with the current
 * string zero terminated.
 *
 * 		Note:  	the string will not be valid, and may not be terminated 
 * 			after subsequent adds, clears or frees.
 *
 * 		Note:   the returned string should not be freed directly.
 */
char *sb_current(string_builder sb);

/*
 * Creates a duplicate of the current string
*/
char *sb_copy(string_builder sb);

/*
 * Clears the current string but maintains the existing buffer
*/
void sb_clear(string_builder sb);

/*
 * Frees the string_builder's resources
*/
void sb_free(string_builder sb);

#endif
