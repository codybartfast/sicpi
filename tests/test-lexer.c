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

void lxr_free_source_test(void)
{
	source src = source_string("123", "test-lexer");
	lexer lxr = lexer_new(src);
	lexer_read(lxr);
	char *name = src->name;

	lexer_free_source(lxr);
	TEST_ASSERT_NULL(lxr->source);
	TEST_ASSERT_FALSE(name == src->name);
}

void lxr_free(void)
{
	source src = source_string("", "lxr_free");
	lexer lxr = lexer_new(src);
	TEST_ASSERT_NOT_NULL(lxr->source);
	TEST_ASSERT_NOT_NULL(lxr->text);
	TEST_ASSERT_NOT_NULL(lxr->temp);

	char *name = src->name;

	string_builder text = lxr->text;
	char *text_buffer = text->buff;

	string_builder temp = lxr->temp;
	char *temp_buffer = temp->buff;

	lexer_free(lxr);

	TEST_ASSERT_TRUE(name == src->name);

	TEST_ASSERT_FALSE(text == lxr->text);
	TEST_ASSERT_FALSE(text_buffer == text->buff);

	TEST_ASSERT_FALSE(temp == lxr->temp);
	TEST_ASSERT_FALSE(temp_buffer == temp->buff);
}

void expected_token(token tkn, enum token_type type, char *text, long offset, long y,
		    long x)
{
	TEST_ASSERT_NOT_NULL(tkn);
	TEST_ASSERT_EQUAL_INT(type, tkn_type(tkn));
	TEST_ASSERT_EQUAL_STRING(text, tkn_text(tkn));
	TEST_ASSERT_EQUAL_INT64(offset, tkn_offset(tkn));
	TEST_ASSERT_EQUAL_INT64(y, tkn_y(tkn));
	TEST_ASSERT_EQUAL_INT64(x, tkn_x(tkn));
}

void lxr_invalid_char_in_identifier(void)
{
	source src = source_string("ident#fier", "test-lexer");
	lexer lxr = lexer_new(src);
	token tkn;

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_ERROR, "ident#", 0, 0, 0);
	TEST_ASSERT_EQUAL_STRING(
		"Unexpected character '#', 0x23, in identifier: 'ident#'.",
		tkn_err_msg(tkn));
	TEST_ASSERT_TRUE(lexer_is_errored(lxr));
}

void lxr_bad_token_start(void)
{
	source src = source_string("#identfier", "blah");
	lexer lxr = lexer_new(src);
	token tkn;

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_ERROR, "#", 0, 0, 0);
	TEST_ASSERT_EQUAL_STRING(
		"Unexpected character '#', 0x23, at start of token: '#'.",
		tkn_err_msg(tkn));
	TEST_ASSERT_TRUE(lexer_is_errored(lxr));
}

void lxr_returns_error_after_returning_error(void)
{
	source src = source_string("123.456.789", "test-lexer");
	lexer lxr = lexer_new(src);
	token tkn;

	tkn = lexer_read(lxr);
	TEST_ASSERT_TRUE(tkn_type(tkn) == TOKEN_ERROR);
	TEST_ASSERT_TRUE(lexer_is_errored(lxr));
	TEST_ASSERT_EQUAL_STRING(
		"Unexpected character '.', 0x2E, in number: '123.456.'.",
		tkn_err_msg(tkn));

	tkn = lexer_read(lxr);
	TEST_ASSERT_TRUE(tkn_type(tkn) == TOKEN_ERROR);
	TEST_ASSERT_TRUE(lexer_is_errored(lxr));
	TEST_ASSERT_EQUAL_STRING(
		"Attempted to read from a lexer after a preceeding error.",
		tkn_err_msg(tkn));

	tkn = lexer_read(lxr);
	TEST_ASSERT_TRUE(tkn_type(tkn) == TOKEN_ERROR);
	TEST_ASSERT_TRUE(lexer_is_errored(lxr));
	TEST_ASSERT_EQUAL_STRING(
		"Attempted to read from a lexer after a preceeding error.",
		tkn_err_msg(tkn));
}

void lxr_dot(void)
{
	source src = source_string(". 456", "dot");
	lexer lxr = lexer_new(src);
	token tkn;

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_DOT, ".", 0, 0, 0);
}

void lxr_dot_bad(void)
{
	source src = source_string(".blah", "dot other");
	lexer lxr = lexer_new(src);
	token tkn;

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_ERROR, ".b", 0, 0, 0);
	TEST_ASSERT_EQUAL_STRING(
		"Unexpected character 'b', 0x62, in dot: '.b'.",
		tkn_err_msg(tkn));
}

void lxr_plus_sub(void)
{
	source src = source_string("+ .567", "blah");
	lexer lxr = lexer_new(src);
	token tkn;

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_IDENTIFIER, "+", 0, 0, 0);

	tkn = lexer_read(lexer_new(source_string("- .678", "blah")));
	expected_token(tkn, TOKEN_IDENTIFIER, "-", 0, 0, 0);
}

void lxr_plus_sub_bad(void)
{
	source src = source_string("+plus", "blah");
	lexer lxr = lexer_new(src);
	token tkn;

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_ERROR, "+p", 0, 0, 0);

	tkn = lexer_read(lexer_new(source_string("-minus", "blah")));
	expected_token(tkn, TOKEN_ERROR, "-m", 0, 0, 0);
}

void lxr_read(void)
{
	source src = source_file(file_path);
	TEST_ASSERT_NOT_NULL(src);
	lexer lxr = lexer_new(src);
	TEST_ASSERT_NOT_NULL(lxr);
	token tkn;

	tkn = lexer_read(lxr);
	TEST_ASSERT_NOT_NULL(tkn);
	expected_token(tkn, TOKEN_LIST_OPEN, "(", 50, 1, 0);

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_IDENTIFIER, "define", 51, 1, 1);

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_IDENTIFIER, "pi", 58, 1, 8);

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_NUMBER, "3.14159", 61, 1, 11);

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_LIST_CLOSE, ")", 68, 1, 18);

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_LIST_OPEN, "(", 88, 2, 1);

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_IDENTIFIER, "define", 90, 2, 3);

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_IDENTIFIER, "radius", 122, 3, 1);

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_NUMBER, "10", 138, 4, 8);

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_LIST_CLOSE, ")", 141, 4, 11);

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_LIST_OPEN, "(", 144, 5, 0);

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_IDENTIFIER, "*", 145, 5, 1);

	int i;
	for (i = 0; i < 17; i++) {
		lexer_read(lxr);
	}

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_IDENTIFIER, "circumference", 208, 7, 0);

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_EOF, "", 278, 8, 56);

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_EOF, "", 278, 8, 56);

	TEST_ASSERT_FALSE(lexer_is_errored(lxr));
}

void lxr_invalid_char_in_number(void)
{
	source src = source_string("123x456", "test-lexer");
	lexer lxr = lexer_new(src);
	token tkn;

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_ERROR, "123x", 0, 0, 0);
	TEST_ASSERT_EQUAL_STRING(
		"Unexpected character 'x', 0x78, in number: '123x'.",
		tkn_err_msg(tkn));
	TEST_ASSERT_TRUE(lexer_is_errored(lxr));
}

void lxr_number_ends_with_dot(void)
{
	source src = source_string("123.", "test-lexer");
	lexer lxr = lexer_new(src);
	token tkn;

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_ERROR, "123.", 0, 0, 0);
	TEST_ASSERT_EQUAL_STRING(
		"Unexpected character '.', 0x2E, at end of number: '123.'.",
		tkn_err_msg(tkn));
	TEST_ASSERT_TRUE(lexer_is_errored(lxr));
}

void lxr_number_with_two_dots(void)
{
	source src = source_string("123.456.789", "test-lexer");
	lexer lxr = lexer_new(src);
	token tkn;

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_ERROR, "123.456.", 0, 0, 0);
	TEST_ASSERT_EQUAL_STRING(
		"Unexpected character '.', 0x2E, in number: '123.456.'.",
		tkn_err_msg(tkn));
	TEST_ASSERT_TRUE(lexer_is_errored(lxr));
}

void lxr_naked_decimal(void)
{
	source src = source_string(".456", "naked_decimal");
	lexer lxr = lexer_new(src);
	token tkn;

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_NUMBER, ".456", 0, 0, 0);
}

void lxr_signed_number(void)
{
	source src = source_string("+12.34", "signed_nuber");
	lexer lxr = lexer_new(src);
	token tkn;

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_NUMBER, "+12.34", 0, 0, 0);

	tkn = lexer_read(
		lexer_new(source_string("-23.45", "negative signed number")));
	expected_token(tkn, TOKEN_NUMBER, "-23.45", 0, 0, 0);
}

void lxr_signed_naked_decimal(void)
{
	source src = source_string("+.567", "pos naked decimal");
	lexer lxr = lexer_new(src);
	token tkn;

	tkn = lexer_read(lxr);
	expected_token(tkn, TOKEN_NUMBER, "+.567", 0, 0, 0);

	tkn = lexer_read(
		lexer_new(source_string("-.678", "neg naked decimal")));
	expected_token(tkn, TOKEN_NUMBER, "-.678", 0, 0, 0);
}

void lxr_string(void)
{
	token tkn;

	tkn = lexer_read(
		lexer_new(source_string("\"Simple String\"", "Simple String")));
	expected_token(tkn, TOKEN_STRING, "Simple String", 0, 0, 0);

	tkn = lexer_read(lexer_new(
		source_string("\"Simple \n String\"", "Simple String")));
	expected_token(tkn, TOKEN_STRING, "Simple \n String", 0, 0, 0);

	tkn = lexer_read(lexer_new(
		source_string("\"Simple \\\" String\"", "Simple String")));

	tkn = lexer_read(lexer_new(
		source_string("\"Simple \\\\ String\"", "Simple String")));
	expected_token(tkn, TOKEN_STRING, "Simple \\ String", 0, 0, 0);

	tkn = lexer_read(lexer_new(
		source_string("\"Simple \\x  String\"", "Simple String")));
	expected_token(tkn, TOKEN_ERROR, "Simple ", 0, 0, 0);
	TEST_ASSERT_EQUAL_STRING(
		"Unexpected character 'x', 0x78, following \\ (backslash) in string: 'Simple '.",
		tkn_err_msg(tkn));

	tkn = lexer_read(
		lexer_new(source_string("\"Simple String", "Simple String")));
	expected_token(tkn, TOKEN_ERROR, "Simple String", 0, 0, 0);
	TEST_ASSERT_EQUAL_STRING(
		"No closing double quote '\"' found for string starting: 'Simple String'.",
		tkn_err_msg(tkn));
}

void lxr_quote(void)
{
	token tkn;

	tkn = lexer_read(lexer_new(source_string("'apple", "foo")));
	expected_token(tkn, TOKEN_QUOTE, "'", 0, 0, 0);

	tkn = lexer_read(lexer_new(source_string("'123", "foo")));
	expected_token(tkn, TOKEN_QUOTE, "'", 0, 0, 0);

	tkn = lexer_read(lexer_new(source_string("'(apple)", "foo")));
	expected_token(tkn, TOKEN_QUOTE, "'", 0, 0, 0);

	tkn = lexer_read(lexer_new(source_string("' apple", "foo")));
	expected_token(tkn, TOKEN_ERROR, "'", 0, 0, 0);
	TEST_ASSERT_EQUAL_STRING(
		"Unexpected character ' ', 0x20, immediatedly after quote: '''.",
		tkn_err_msg(tkn));

	tkn = lexer_read(lexer_new(source_string("` apple", "foo")));
	expected_token(tkn, TOKEN_ERROR, "`", 0, 0, 0);
	TEST_ASSERT_EQUAL_STRING(
		"Unexpected character ' ', 0x20, immediatedly after quasiquote: '`'.",
		tkn_err_msg(tkn));

	tkn = lexer_read(lexer_new(source_string(", apple", "foo")));
	expected_token(tkn, TOKEN_ERROR, ",", 0, 0, 0);
	TEST_ASSERT_EQUAL_STRING(
		"Unexpected character ' ', 0x20, immediatedly after unquote: ','.",
		tkn_err_msg(tkn));

	tkn = lexer_read(lexer_new(source_string("'", "foo")));
	expected_token(tkn, TOKEN_ERROR, "'", 0, 0, 0);
	TEST_ASSERT_EQUAL_STRING("Expected an expression after '''.",
				 tkn_err_msg(tkn));
}

void lxr_multiquote(void)
{
	lexer lxr = lexer_new(
		source_string("'apple `banana ,cherry\n`(list) ,123", "foo"));

	expected_token(lexer_read(lxr), TOKEN_QUOTE, "'", 0, 0, 0);
	expected_token(lexer_read(lxr), TOKEN_IDENTIFIER, "apple", 1, 0, 1);

	expected_token(lexer_read(lxr), TOKEN_QUASIQUOTE, "`", 7, 0, 7);
	expected_token(lexer_read(lxr), TOKEN_IDENTIFIER, "banana", 8, 0, 8);

	expected_token(lexer_read(lxr), TOKEN_UNQUOTE, ",", 15, 0, 15);
	expected_token(lexer_read(lxr), TOKEN_IDENTIFIER, "cherry", 16, 0, 16);

	expected_token(lexer_read(lxr), TOKEN_QUASIQUOTE, "`", 23, 1, 0);
	expected_token(lexer_read(lxr), TOKEN_LIST_OPEN, "(", 24, 1, 1);
	expected_token(lexer_read(lxr), TOKEN_IDENTIFIER, "list", 25, 1, 2);
	expected_token(lexer_read(lxr), TOKEN_LIST_CLOSE, ")", 29, 1, 6);

	expected_token(lexer_read(lxr), TOKEN_UNQUOTE, ",", 31, 1, 8);
	expected_token(lexer_read(lxr), TOKEN_NUMBER, "123", 32, 1, 9);

	expected_token(lexer_read(lxr), TOKEN_EOF, "\0", 34, 1, 11);
	expected_token(lexer_read(lxr), TOKEN_EOF, "\0", 34, 1, 11);
}

void lxr_text(void)
{
	lexer lxr = lexer_new(
		source_string("'apple `banana ,cherry\n`(list) ,123", "foo"));

	while (lexer_read(lxr)->type > TOKEN_UNDEFINED)
		;

	TEST_ASSERT_EQUAL_STRING("'apple `banana ,cherry\n`(list) ,123",
				 sb_current(lxr->text));
}

void lxr_sets_token_source(void)
{
	token_source tkn_src;
	lexer lxr = lexer_new(source_string("durian", ""));
	lexer_set_token_source(lxr, &tkn_src);
	TEST_ASSERT_EQUAL_PTR(lxr, tkn_src.state);
	TEST_ASSERT_EQUAL_PTR(lexer_read, tkn_src.read_token);
}

int test_lexer(void)
{
	RUN_TEST(lxr_new_null_src);
	RUN_TEST(lxr_new);
	RUN_TEST(lxr_free_null);
	RUN_TEST(lxr_free_source_test);
	RUN_TEST(lxr_free);
	RUN_TEST(lxr_invalid_char_in_identifier);
	RUN_TEST(lxr_bad_token_start);
	RUN_TEST(lxr_dot);
	RUN_TEST(lxr_dot_bad);
	RUN_TEST(lxr_plus_sub);
	RUN_TEST(lxr_plus_sub_bad);
	RUN_TEST(lxr_returns_error_after_returning_error);
	RUN_TEST(lxr_read);
	RUN_TEST(lxr_invalid_char_in_number);
	RUN_TEST(lxr_number_ends_with_dot);
	RUN_TEST(lxr_number_with_two_dots);
	RUN_TEST(lxr_naked_decimal);
	RUN_TEST(lxr_signed_number);
	RUN_TEST(lxr_signed_naked_decimal);
	RUN_TEST(lxr_string);
	RUN_TEST(lxr_quote);
	RUN_TEST(lxr_multiquote);
	RUN_TEST(lxr_text);
	RUN_TEST(lxr_sets_token_source);
	return 0;
}
