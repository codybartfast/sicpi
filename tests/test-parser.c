
#include "../unity/src/unity.h"

#include "../src/lexer.h"
#include "../src/parser.h"

#include <stdbool.h>

void test_parser_integer(void){
	token_source tkn_src;

	lexer lxr = lexer_new(source_string("123 +234 -345 04560 -08030", ""));
	lexer_set_token_source(lxr, &tkn_src);
	object rslt;

	rslt = parse(tkn_src);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_INT64(123, to_integer(rslt));

	rslt = parse(tkn_src);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_INT64(234, to_integer(rslt));

	rslt = parse(tkn_src);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_INT64(-345, to_integer(rslt));

	rslt = parse(tkn_src);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_INT64(4560, to_integer(rslt));

	rslt = parse(tkn_src);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_INT64(-8030, to_integer(rslt));

	lexer_free_source(lxr);
	lexer_free(lxr);
}

void test_parser_decimal(void){
	token_source tkn_src;

	lexer lxr = lexer_new(source_string("12.3 +23.4 -34.5 0.4560 -.08030", ""));
	lexer_set_token_source(lxr, &tkn_src);
	object rslt;

	rslt = parse(tkn_src);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_DOUBLE(12.3, to_floating(rslt));

	// rslt = parse(tkn_src);
	// TEST_ASSERT_TRUE(is_number(rslt));
	// TEST_ASSERT_EQUAL_INT64(23.4, to_integer(rslt));

	// rslt = parse(tkn_src);
	// TEST_ASSERT_TRUE(is_number(rslt));
	// TEST_ASSERT_EQUAL_INT64(-34.5, to_integer(rslt));

	// rslt = parse(tkn_src);
	// TEST_ASSERT_TRUE(is_number(rslt));
	// TEST_ASSERT_EQUAL_INT64(0.4560, to_integer(rslt));

	// rslt = parse(tkn_src);
	// TEST_ASSERT_TRUE(is_number(rslt));
	// TEST_ASSERT_EQUAL_INT64(-.08030, to_integer(rslt));

	lexer_free_source(lxr);
	lexer_free(lxr);}

int test_parser(void)
{
	RUN_TEST(test_parser_integer);
	RUN_TEST(test_parser_decimal);
	return 0;
}
