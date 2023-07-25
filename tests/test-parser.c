#include "../unity/src/unity.h"

#include "../src/lexer.h"
#include "../src/parser.h"

#include <stdbool.h>

void hello_parser(void){
	token_source tkn_src;
	lexer lxr = lexer_new(source_string("486", ""));
	lexer_set_token_source(lxr, &tkn_src);
	object rslt = parse(tkn_src);
	TEST_ASSERT_TRUE(is_number(rslt));
	TEST_ASSERT_EQUAL_INT64(486, to_integer(rslt));
	// TEST_ASSERT_EQUAL_STRING("blah", tkn_text(lexer_read(lxr)));
}

int test_parser(void)
{
	RUN_TEST(hello_parser);
	return 0;
}
