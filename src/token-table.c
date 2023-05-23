#include "token-table.h"

#include <stdlib.h>

const size_t growth_factor = 2;

token_table token_table_new(size_t initial_capacity)
{
	initial_capacity = initial_capacity < 1 ?
				   TOKEN_TABLE_DEFAULT_INITIAL_CAPACITY :
				   initial_capacity;
	token_table tt = malloc(sizeof(struct token_table));
	token *table = malloc(sizeof(token *) * initial_capacity);

	if (tt == NULL || table == NULL) {
		alloc_error("new_token_table");
	}

	tt->table = table;
	tt->next = table;
	tt->table_end = table + initial_capacity;
	return tt;
}

void grow_table(token_table tt)
{
	token *new_table;
	static size_t sizeLimit = __SIZE_MAX__;
	static size_t fullGrowLimit = __SIZE_MAX__ / growth_factor;
	size_t old_alloc = sizeof(token) * (tt->table_end - tt->table);
	if (old_alloc == sizeLimit) {
		alloc_error("token_table grow (limit)");
	}
	size_t new_alloc = old_alloc > fullGrowLimit ?
				   sizeLimit :
				   old_alloc * growth_factor;
	new_table = realloc(tt->table, new_alloc);
	if (!new_table) {
		alloc_error("token_table grow");
	}
	token *new_next = new_table + (tt->next - tt->table);

	tt->table = new_table;
	tt->next = new_next;
	tt->table_end = new_table + (new_alloc / sizeof(token));
}

TOKEN_TABLE_KEY_T token_table_add(token_table tt, token t)
{
	if (tt->next >= tt->table_end) {
		grow_table(tt);
	}
	size_t usedCount = tt->next - tt->table;
	if (usedCount == TOKEN_TABLE_KEY_MAX) {
		eprintfx("Token Table is full");
	}
	TOKEN_TABLE_KEY_T key = usedCount;
	*tt->next = t;
	++tt->next;
	return key;
}

token token_table_get(token_table tt, TOKEN_TABLE_KEY_T key)
{
	return *(tt->table + key);
}

void token_table_free(token_table tt)
{
	if (tt) {
		for (token *t = tt->table; t < tt->next; ++t) {
			free(*t);
		}
		free(tt->table);
		tt->table = NULL;
		tt->next = NULL;
		tt->table_end = NULL;
		free(tt);
	}
}
