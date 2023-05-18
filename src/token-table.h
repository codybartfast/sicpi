#ifndef TOKEN_TABLE_H
#define TOKEN_TABLE_H

#include <stdint.h>

#include "sicpstd.h"
#include "token.h"

#define TOKEN_TABLE_DEFAULT_INITIAL_CAPACITY 1024
#define TOKEN_TABLE_IDX_T uint32_t

typedef struct token_table {
	token *table;
	token *next;
	token *table_end;
} *token_table;

token_table token_table_new(size_t initial_capacity);
TOKEN_TABLE_IDX_T token_table_add(token_table tt, token tkn);
token token_table_get(token_table tt, TOKEN_TABLE_IDX_T idx);
void token_table_free(token_table tt);

#endif
