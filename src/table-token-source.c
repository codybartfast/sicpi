#include "table-token-source.h"
#include "sicp-std.h"

token token_table_souce_read_token(void *state);

// Todo: ? rather than 'new' we should probably have an init like
// token_source_init
// Todo: Free


typedef struct lexer_and_table {
	struct lexer *lexer;
	struct token_table *table;
} lexer_and_table;

void token_table_source_free(token_source tkn_src)
{
	if(tkn_src) {
		free(tkn_src->state);
		free(tkn_src);
	}
}

token_source table_token_source_new(lexer lxr, token_table table)
{
	// ToDo: check for nulls?
	struct lexer_and_table *lat = mallocx(sizeof(struct lexer_and_table),
					      "token_table_source_new");
	token_source tkn_src =
		mallocx(sizeof(struct token_source), "token_table_source_new");
	lat->lexer = lxr;
	lat->table = table;
	tkn_src->state = lat;
	tkn_src->read_token = token_table_souce_read_token;
	return tkn_src;
}

token token_table_souce_read_token(void *state)
{
	struct lexer_and_table *lat = state;
	token tkn = lexer_read(lat->lexer);
	uint32_t key = token_table_add(lat->table, tkn);
	tkn->key = key;
	return tkn;
}

