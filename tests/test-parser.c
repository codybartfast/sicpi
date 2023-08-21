
#include "../unity/src/unity.h"

#include "../src/lexer.h"
#include "../src/parser.h"

#include <stdbool.h>

void init(parser parser, token_source tkn_src){
	parser_init(parser);
	parser->token_source = tkn_src;
}

void test_parser_integer(void)
{
	lexer lxr = lexer_new(source_string("123 +234 -345 04560 -08030", ""));

	struct token_source tkn_src;
	struct parser parser;
	object rslt;
	init(&parser, &tkn_src);
	lexer_set_token_source(lxr, &tkn_src);

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_INT64(123, to_integer(rslt));

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_INT64(234, to_integer(rslt));

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_INT64(-345, to_integer(rslt));

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_INT64(4560, to_integer(rslt));

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_INT64(-8030, to_integer(rslt));

	lexer_free_source(lxr);
	lexer_free(lxr);
}

void test_parser_decimal(void)
{
	lexer lxr =
		lexer_new(source_string("12.3 +23.4 -34.5 0.4560 -.08030", ""));

	struct token_source tkn_src;
	struct parser parser;
	object rslt;
	init(&parser, &tkn_src);
	lexer_set_token_source(lxr, &tkn_src);

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_DOUBLE(12.3, to_floating(rslt));

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_DOUBLE(23.4, to_floating(rslt));

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_DOUBLE(-34.5, to_floating(rslt));

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_DOUBLE(0.4560, to_floating(rslt));

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_DOUBLE(-0.08030, to_floating(rslt));

	lexer_free_source(lxr);
	lexer_free(lxr);
}

int test_parser(void)
{
	RUN_TEST(test_parser_integer);
	RUN_TEST(test_parser_decimal);
	return 0;
}
