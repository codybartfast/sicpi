#include "../unity/src/unity.h"
#include "../src/table-token-source.h"
#include "../src/parser.h"


#include <string.h>

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

void test_table_token_source(void)
{
	RUN_TEST(token_table_source_pair_has_token_of_car);
}
