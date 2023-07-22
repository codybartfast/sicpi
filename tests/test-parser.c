#include "../unity/src/unity.h"

#include "../src/parser.h"

void hello_parser(void){
	TEST_ASSERT_TRUE(0);
}

int test_parser(void)
{
	RUN_TEST(hello_parser);
	return 0;
}
