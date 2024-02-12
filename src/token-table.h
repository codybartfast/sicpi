#ifndef TOKEN_TABLE_H
#define TOKEN_TABLE_H

#include "lexer.h"
#include "sicp-error.h"
#include "token.h"

#include <limits.h>
#include <stdint.h>

/*
 * This is intended to store references to tokens created by the lexer so they
 * can be referred to by scheme objects using a key.
 *
 * If nothing else, this allows control over the amount of space used to refer
 * to the token in the scheme object.
 */

#define TOKEN_TABLE_DEFAULT_INITIAL_CAPACITY 1024

/*
 * The integer type used for table key/index.
 * Changes to this may need be reflected in token->key
 */
#define TOKEN_TABLE_KEY_T uint32_t
#define TOKEN_TABLE_KEY_MAX UINT32_MAX

typedef struct token_table {
	token *table;
	token *next;
	token *table_end;
} *token_table;

/* Create a new token table */
token_table token_table_new(size_t initial_capacity);

/* Add a token to the table - returns the key of the token in table. */
TOKEN_TABLE_KEY_T token_table_add(token_table tt, token tkn);

/* Get a token from a table using its key */
token token_table_get(token_table tt, TOKEN_TABLE_KEY_T key);

/*
 * Not implemented - Maybe tokens could removed from the table and have their
 * memory freed when all scheme objects that reference a token are garbage
 * collected.
 *
 * This would also mean their keys could be reused.  But probably need to
 * compact remaining tokens and update references from objects.
 */
// void token_table_remove (token_table tt, TOKEN_TABLE_KEY_T key);

/* Frees memory used by the table and the tokens it references */
void token_table_free(token_table tt);

#endif
