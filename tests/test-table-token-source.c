#include "../unity/src/unity.h"
#include "../src/table-token-source.h"
#include "../src/parser.h"
#include "../src/explicit-control-evaluator.h" // ToDo: Isn't this a unit test?

// ToDo: wip?
void tts_wip(void)
{
	lexer lxr = lexer_new(source_string("(+ 1 2 3)", ""));
	token_table table = token_table_new(0);
	token_source tts = table_token_source_new(lxr, table);
	struct parser parser;
	parser_init(&parser, tts);
	object rslt = EC_Eval(parse(&parser));
	TEST_ASSERT_EQUAL_STRING(to_text(rslt), "6");
}

void test_table_token_source(void)
{
	RUN_TEST(tts_wip);
}
