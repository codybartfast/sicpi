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
	source src = source_string("", "");
	lexer lxr = lexer_new(src);
	TEST_ASSERT_NOT_NULL(lxr);
	TEST_ASSERT_EQUAL_size_t(0, strlen(sb_current(lxr->text)));
	TEST_ASSERT_EQUAL_size_t(0, strlen(sb_current(lxr->temp)));
	TEST_ASSERT_EQUAL_PTR(src, lxr->source);
}

void lxr_free_null(void)
{
	lexer_free(NULL);
}

void lxr_free(void)
{
	source src = source_string("", "");
	lexer lxr = lexer_new(src);
	TEST_ASSERT_NOT_NULL(lxr->text);
	TEST_ASSERT_NOT_NULL(lxr->temp);
	string_builder text = lxr->text;
	string_builder temp = lxr->temp;
	char *text_buffer = text->buff;
	char *temp_buffer = temp->buff;
	lexer_free(lxr);
	TEST_ASSERT_FALSE(text == lxr->text);
	TEST_ASSERT_FALSE(temp == lxr->temp);
	TEST_ASSERT_FALSE(text_buffer == text->buff);
	TEST_ASSERT_FALSE(temp_buffer == temp->buff);
}

void expected_token(token tkn, token_type type, char *text, long offset, long y,
		    long x)
{
	TEST_ASSERT_NOT_NULL(tkn);
	TEST_ASSERT_EQUAL_INT(type, tkn_type(tkn));
	TEST_ASSERT_EQUAL_STRING(text, tkn_text(tkn));
	TEST_ASSERT_EQUAL_INT64(offset, tkn_offset(tkn));
	TEST_ASSERT_EQUAL_INT64(y, tkn_y(tkn));
	TEST_ASSERT_EQUAL_INT64(x, tkn_x(tkn));
}

void lxr_read(void)
{
	source src = source_file(file_path);
	TEST_ASSERT_NOT_NULL(src);
	lexer lxr = lexer_new(src);
	TEST_ASSERT_NOT_NULL(lxr);
	token tkn;

	tkn = lexer_read(lxr);
	expected_token(tkn, TKN_LIST_OPEN, "(", 50, 1, 0);

	tkn = lexer_read(lxr);
	expected_token(tkn, TKN_IDENTIFIER, "define", 51, 1, 1);
}

void lxr_invalid_char_in_identifier(void)
{
	TEST_FAIL();
}
// void lxr_read_after_error
// void lxr_free_source

int test_lexer(void)
{
	RUN_TEST(lxr_new_null_src);
	RUN_TEST(lxr_new);
	RUN_TEST(lxr_free_null);
	RUN_TEST(lxr_free);
	RUN_TEST(lxr_read);
	RUN_TEST(lxr_invalid_char_in_identifier);
}
