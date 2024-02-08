#ifndef TABLE_TOKEN_SOURCE_H
#define TABLE_TOKEN_SOURCE_H

#include "lexer.h"
#include "token.h"
#include "token-table.h"

token_source table_token_source_new(lexer lxr, token_table table);

#endif
