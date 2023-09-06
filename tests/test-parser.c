
#include "../unity/src/unity.h"

#include "../src/lexer.h"
#include "../src/parser.h"
#include "../src/obarray.h"

#include <stdbool.h>

void init(lexer lxr, token_source tkn_src, parser parser)
{
	lexer_set_token_source(lxr, tkn_src);
	parser_init(parser, tkn_src);
}

void free_lexer(lexer lxr)
{
	lexer_free_source(lxr);
	lexer_free(lxr);
}

void test_parser_catch_errors(void)
{
	lexer lxr = lexer_new(source_string("1#1 1", ""));
	struct token_source tkn_src;
	struct parser parser;
	init(lxr, &tkn_src, &parser);
	object rslt;

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_error(rslt));
	TEST_ASSERT_EQUAL_INT(ERROR_LEXER, to_error_kind(rslt));
	TEST_ASSERT_TRUE(parser_is_errored(&parser));

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_error(rslt));
	TEST_ASSERT_EQUAL_INT(ERROR_PARSER, to_error_kind(rslt));
	TEST_ASSERT_TRUE(parser_is_errored(&parser));
	TEST_ASSERT_EQUAL_STRING("Attempted to parse after an earlier error.",
				 parser_error_message(&parser));

	free_lexer(lxr);
}

void test_parser_end_of_source(void)
{
	lexer lxr = lexer_new(source_string("123", ""));
	struct token_source tkn_src;
	struct parser parser;
	init(lxr, &tkn_src, &parser);
	object rslt;

	rslt = parse(&parser); // ignore number

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(eq(EOS, rslt));

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(eq(EOS, rslt));

	free_lexer(lxr);
}

void test_parser_integer(void)
{
	lexer lxr = lexer_new(source_string("123 +234 -345 04560 -08030", ""));
	struct token_source tkn_src;
	struct parser parser;
	init(lxr, &tkn_src, &parser);
	object rslt;
	;

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

	free_lexer(lxr);
}

void test_parser_decimal(void)
{
	lexer lxr =
		lexer_new(source_string("12.3 +23.4 -34.5 0.4560 -.08030", ""));
	struct token_source tkn_src;
	struct parser parser;
	init(lxr, &tkn_src, &parser);
	object rslt;

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

	free_lexer(lxr);
}

void test_parser_string(void)
{
	lexer lxr = lexer_new(source_string("\"Interested\" \"Fox\"", ""));
	struct token_source tkn_src;
	struct parser parser;
	init(lxr, &tkn_src, &parser);
	object rslt;

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_string(rslt));
	TEST_ASSERT_EQUAL_STRING("Interested", to_string(rslt));

	rslt = parse(&parser);
	TEST_ASSERT_TRUE(is_string(rslt));
	TEST_ASSERT_EQUAL_STRING("Fox", to_string(rslt));

	free_lexer(lxr);
}

void test_parser_list(void)
{
	lexer lxr = lexer_new(source_string("() (a) (b c) (d e f)", ""));
	struct token_source tkn_src;
	struct parser parser;
	init(lxr, &tkn_src, &parser);
	object obj;

	obj = parse(&parser);
	TEST_ASSERT_EQUAL(EMPTY_LIST, obj);

	obj = parse(&parser);
	TEST_ASSERT_EQUAL_STRING("a", to_name(car(obj)));
	TEST_ASSERT_EQUAL(EMPTY_LIST, cdr(obj));

	obj = parse(&parser);
	TEST_ASSERT_EQUAL_STRING("b", to_name(car(obj)));
	TEST_ASSERT_EQUAL_STRING("c", to_name(car(cdr(obj))));
	TEST_ASSERT_EQUAL(EMPTY_LIST, cdr(cdr(obj)));

	obj = parse(&parser);
	TEST_ASSERT_EQUAL_STRING("d", to_name(car(obj)));
	TEST_ASSERT_EQUAL_STRING("e", to_name(car(cdr(obj))));
	TEST_ASSERT_EQUAL_STRING("f", to_name(car(cdr(cdr(obj)))));
	TEST_ASSERT_EQUAL(EMPTY_LIST, cdr(cdr(cdr(obj))));

	obj = parse(&parser);
	TEST_ASSERT_EQUAL(EOS, obj);

	lexer_free_source(lxr);
	lexer_free(lxr);
	lxr = lexer_new(source_string("(a b c", ""));
	init(lxr, &tkn_src, &parser);
	obj = parse(&parser);
	TEST_ASSERT_TRUE(is_error(obj));
	TEST_ASSERT_EQUAL_STRING("List was not closed before end of file.",
				 parser_error_message(&parser));

	lexer_free_source(lxr);
	lexer_free(lxr);
	lxr = lexer_new(source_string("(a b#B c)", ""));
	init(lxr, &tkn_src, &parser);
	obj = parse(&parser);
	TEST_ASSERT_TRUE(is_error(obj));
	TEST_ASSERT_EQUAL_STRING("Lexer error.", parser_error_message(&parser));

	lexer_free_source(lxr);
	lexer_free(lxr);
	lxr = lexer_new(source_string("(a) )", ""));
	init(lxr, &tkn_src, &parser);
	obj = parse(&parser);
	obj = parse(&parser);
	TEST_ASSERT_TRUE(is_error(obj));
	TEST_ASSERT_EQUAL_STRING("List close ')' was not expected.",
				 parser_error_message(&parser));

	free_lexer(lxr);
}

void test_parser_identifiers(void)
{
	lexer lxr = lexer_new(source_string(" apple banana apple banana ", ""));
	struct token_source tkn_src;
	struct parser parser;
	init(lxr, &tkn_src, &parser);

	object apple1 = parse(&parser);
	TEST_ASSERT_TRUE(is_symbol(apple1));
	TEST_ASSERT_EQUAL_STRING("apple", to_name(apple1));

	object banana1 = parse(&parser);
	TEST_ASSERT_TRUE(is_symbol(banana1));
	TEST_ASSERT_EQUAL_STRING("banana", to_name(banana1));

	object apple2 = parse(&parser);
	TEST_ASSERT_TRUE(is_symbol(apple2));
	TEST_ASSERT_EQUAL_STRING("apple", to_name(apple2));

	object banana2 = parse(&parser);
	TEST_ASSERT_TRUE(is_symbol(banana2));
	TEST_ASSERT_EQUAL_STRING("banana", to_name(banana2));

	TEST_ASSERT_EQUAL_PTR(apple1, apple2);
	TEST_ASSERT_EQUAL_PTR(banana1, banana2);
	TEST_ASSERT_FALSE(apple1 == banana1);

	free_lexer(lxr);
}

void test_parser_keywords(void)
{
	lexer lxr = lexer_new(source_string("quote unquote", ""));
	struct token_source tkn_src;
	struct parser parser;
	init(lxr, &tkn_src, &parser);
	object obj;

	obj = parse(&parser);
	TEST_ASSERT_EQUAL(QUOTE, obj);

	obj = parse(&parser);
	TEST_ASSERT_EQUAL(obj, UNQUOTE);

	free_lexer(lxr);
}

void test_parser_quotation(void)
{
	lexer lxr = lexer_new(source_string("'pear `2 ,() '(a b) 'a#a", ""));
	struct token_source tkn_src;
	struct parser parser;
	init(lxr, &tkn_src, &parser);
	object obj;

	obj = parse(&parser);
	TEST_ASSERT_EQUAL(QUOTE, car(obj));
	TEST_ASSERT_EQUAL(from_name("pear", NO_META_DATA), cdr(obj));

	obj = parse(&parser);
	TEST_ASSERT_EQUAL(QUASIQUOTE, car(obj));
	TEST_ASSERT_EQUAL(2, to_integer(cdr(obj)));

	obj = parse(&parser);
	TEST_ASSERT_EQUAL(UNQUOTE, car(obj));
	TEST_ASSERT_EQUAL(EMPTY_LIST, cdr(obj));

	obj = parse(&parser);
	TEST_ASSERT_EQUAL(QUOTE, car(obj));
	TEST_ASSERT_EQUAL(from_name("a", NO_META_DATA), car(cdr(obj)));
	TEST_ASSERT_EQUAL(from_name("b", NO_META_DATA), car(cdr(cdr(obj))));
	TEST_ASSERT_EQUAL(EMPTY_LIST, cdr(cdr(cdr(obj))));

	obj = parse(&parser);
	TEST_ASSERT_TRUE(is_error(obj));

	free_lexer(lxr);
}

void test_parser_dot(void)
{
	lexer lxr = lexer_new(source_string("(proc arg1 arg2 . rest)", ""));
	struct token_source tkn_src;
	struct parser parser;
	init(lxr, &tkn_src, &parser);
	object obj;

	obj = parse(&parser);
	TEST_ASSERT_EQUAL_STRING("proc", to_name(car(obj)));

	obj = cdr(obj);
	TEST_ASSERT_EQUAL_STRING("arg1", to_name(car(obj)));

	obj = cdr(obj);
	TEST_ASSERT_EQUAL_STRING("arg2", to_name(car(obj)));

	obj = cdr(obj);
	TEST_ASSERT_EQUAL(DOT, car(obj));

	obj = cdr(obj);
	TEST_ASSERT_EQUAL_STRING("rest", to_name(car(obj)));

	obj = cdr(obj);
	TEST_ASSERT_EQUAL(EMPTY_LIST, obj);

	lexer_free_source(lxr);
	lexer_free(lxr);
	lxr = lexer_new(source_string("(. rest)", ""));
	init(lxr, &tkn_src, &parser);
	obj = parse(&parser);
	TEST_ASSERT_TRUE(is_error(obj));
	TEST_ASSERT_EQUAL_STRING(
		"Dot '.' can only be used for dotted-tail definitions",
		parser_error_message(&parser));

	lexer_free_source(lxr);
	lexer_free(lxr);
	lxr = lexer_new(source_string("(proc . )", ""));
	init(lxr, &tkn_src, &parser);
	obj = parse(&parser);
	TEST_ASSERT_TRUE(is_error(obj));
	TEST_ASSERT_EQUAL_STRING(
		"Dot '.' can only be used for dotted-tail definitions",
		parser_error_message(&parser));

	lexer_free_source(lxr);
	lexer_free(lxr);
	lxr = lexer_new(source_string("(proc . rest more)", ""));
	init(lxr, &tkn_src, &parser);
	obj = parse(&parser);
	TEST_ASSERT_TRUE(is_error(obj));
	TEST_ASSERT_EQUAL_STRING(
		"Dot '.' can only be used for dotted-tail definitions",
		parser_error_message(&parser));

	free_lexer(lxr);
}

void test_parser_parse_all(void)
{
	lexer lxr = lexer_new(source_string(
		"1 2.0 \"three\" (four (five six) . seven) 8", ""));
	struct token_source tkn_src;
	struct parser parser;
	init(lxr, &tkn_src, &parser);
	object obj;

	obj = parse_all(&parser);
	TEST_ASSERT_EQUAL(1, to_integer(car(obj)));

	obj = cdr(obj);
	TEST_ASSERT_EQUAL(2.0, to_floating(car(obj)));

	obj = cdr(obj);
	TEST_ASSERT_EQUAL_STRING("three", to_string(car(obj)));

	obj = cdr(obj);
	object lst = car(obj);
	TEST_ASSERT_EQUAL_STRING("four", to_name(car(lst)));
	lst = cdr(lst);
	TEST_ASSERT_EQUAL_STRING("five", to_name(car(car(lst))));
	TEST_ASSERT_EQUAL_STRING("six", to_name(car(cdr(car(lst)))));
	TEST_ASSERT_EQUAL(EMPTY_LIST, cdr(cdr(car(lst))));
	lst = cdr(lst);
	TEST_ASSERT_EQUAL(DOT, car(lst));
	lst = cdr(lst);
	TEST_ASSERT_EQUAL_STRING("seven", to_name(car(lst)));

	obj = cdr(obj);
	TEST_ASSERT_EQUAL(8, to_integer(car(obj)));
	TEST_ASSERT_EQUAL(EMPTY_LIST, cdr(obj));

	free_lexer(lxr);
}

int test_parser(void)
{
	RUN_TEST(test_parser_catch_errors);
	RUN_TEST(test_parser_end_of_source);
	RUN_TEST(test_parser_integer);
	RUN_TEST(test_parser_decimal);
	RUN_TEST(test_parser_string);
	RUN_TEST(test_parser_list);
	RUN_TEST(test_parser_identifiers);
	RUN_TEST(test_parser_keywords);
	RUN_TEST(test_parser_quotation);
	RUN_TEST(test_parser_dot);
	RUN_TEST(test_parser_parse_all);
	return 0;
}
