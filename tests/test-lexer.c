#include "../unity/src/unity.h"

#include <string.h>

#include "../src/lexer.h"
#include "../src/source.h"
#include "../src/string-builder.h"

static char file_path[] = "tests/stuff/test-lexer.txt";

void lxr_new_null_src(void)
{
	TEST_ASSERT_NULL(lexer_new(NULL));
}

void lxr_new(void)
{
	source src = source_string("");
	lexer lxr = lexer_new(src);
	TEST_ASSERT_NOT_NULL(lxr);
	TEST_ASSERT_EQUAL_size_t(0, strlen(sb_current(lxr->sb)));
	TEST_ASSERT_EQUAL_PTR(src, lxr->source);
}

void lxr_free_null(void)
{
	lexer_free(NULL);
}

void lxr_free(void)
{
	source src = source_string("");
	lexer lxr = lexer_new(src);
	TEST_ASSERT_NOT_NULL(lxr->sb);
	string_builder strbld = lxr->sb;
	char *buffer = strbld->buff;
	lexer_free(lxr);
	TEST_ASSERT_FALSE(strbld == lxr->sb);
	TEST_ASSERT_FALSE(buffer == strbld->buff);
}

int test_lexer(void)
{
	RUN_TEST(lxr_new_null_src);
	RUN_TEST(lxr_new);
	RUN_TEST(lxr_free_null);
	RUN_TEST(lxr_free);
}
