#ifndef TOKEN_TABLE_H
#define TOKEN_TABLE_H

#include <stdint.h>

#include "sicpstd.h"
#include "token.h"

#define TOKEN_TABLE_IDX uint32_t

typedef struct token_table {
	token *table;
	token *next;
	token *table_end;
} *token_table;

token_table new_token_table(void);
TOKEN_TABLE_IDX token_table_add(token tkn);
void token_table_get(TOKEN_TABLE_IDX idx);

#endif
