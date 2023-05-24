#include "../unity/src/unity.h"

#include "../src/lexer.h"

static char file_path[] = "tests/stuff/test-lexer.txt";

void lxr_new_null_src(void)
{
	TEST_ASSERT_NULL(lexer_new(NULL));
}

int test_lexer(void)
{
	RUN_TEST(lxr_new_null_src);
}
