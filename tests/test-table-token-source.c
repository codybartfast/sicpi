#include "../unity/src/unity.h"
#include "../src/table-token-source.h"
#include "../src/parser.h"
#include "../src/explicit-control-evaluator.h" // ToDo: Isn't this a unit test?

// ToDo: wip?
#include "../src/list.h" // TEMP

#include <string.h>

// A little late realised that we can't reliably put token meta data in symbols
// because 1) some are created before they're used 2) they're interned so all
// instances would have the token for the first instance (e.g. procedure
// definition instead of procedure call).  But we can put the meta data for the
// car of a pair into the pair object.
//
void token_table_source_pair_has_token_of_car(void)
{
	char *src_name = "Jacket";
	lexer lxr = lexer_new(source_string(
		"(apple banana cherry durian elderberry)", src_name));
	token_table table = token_table_new(0);
	token_source tts = table_token_source_new(lxr, table);
	struct parser parser;
	parser_init(&parser, tts);
	for (object expr = parse(&parser); expr != EMPTY_LIST;
	     expr = cdr(expr)) {
		char *obj_txt = to_text(car(expr));
		TOKEN_TABLE_KEY_T key = object_meta_data(expr);
		token tkn = token_table_get(table, key);
		char *tkn_txt = token_text(tkn);

		// Checking we got a token and it's the right one!
		TEST_ASSERT_EQUAL_STRING(obj_txt, tkn_txt);
	}
}

// ToDo: how handle no_meta_data?

void test_table_token_source(void)
{
	RUN_TEST(token_table_source_pair_has_token_of_car);
}
