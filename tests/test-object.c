#include "../src/object.h"
#include "../unity/src/unity.h"

#include <stdbool.h>

void has_one_type(object obj)
{
	int common_count = is_error(obj) + is_number(obj) + is_pair(obj) +
			   is_string(obj) + is_symbol(obj);

	int singleton_count = is_empty_list(obj) + is_eos(obj);

	TEST_ASSERT_EQUAL(1, common_count + singleton_count);
}

void obj_free(void)
{
	object_free(from_integer(0, NO_META_DATA));
}

void obj_error(void)
{
	meta_data meta_data = 125;
	enum error_kind error_kind = ERROR_LEXER;
	object obj = from_error_kind(error_kind, meta_data);

	TEST_ASSERT_TRUE(is_error(obj));
	has_one_type(obj);

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL(error_kind, to_error_kind(obj));
}

void obj_singletons(void)
{
	TEST_ASSERT_TRUE(is_eos(EOS));
	has_one_type(EOS);

	TEST_ASSERT_TRUE(is_empty_list(EMPTY_LIST));
	has_one_type(EMPTY_LIST);
}

void obj_integer(void)
{
	meta_data meta_data = 124;
	integer integer = 486;
	object obj = from_integer(integer, meta_data);

	TEST_ASSERT_TRUE(is_number(obj));
	has_one_type(obj);

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL(integer, to_integer(obj));
}

void obj_floating(void)
{
	meta_data meta_data = 124;
	floating floating = 48.6;
	object obj = from_floating(floating, meta_data);

	TEST_ASSERT_TRUE(is_number(obj));
	has_one_type(obj);

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL(floating, to_floating(obj));
}

void obj_string(void)
{
	meta_data meta_data = 125;
	char *string = "Blue dog";
	object obj = from_string(string, meta_data);

	TEST_ASSERT_TRUE(is_string(obj));
	has_one_type(obj);

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL(string, to_string(obj));
}

void obj_pair(void)
{
	meta_data meta_data = 127;
	object a = from_integer(39, NO_META_DATA);
	object b = from_string("thrity nine", NO_META_DATA);
	object a2 = from_string("forty", NO_META_DATA);
	object b2 = from_integer(40, NO_META_DATA);

	object obj = cons(a, b, meta_data);

	TEST_ASSERT_FALSE(is_pair(EMPTY_LIST));
	TEST_ASSERT_TRUE(is_null(EMPTY_LIST));

	TEST_ASSERT_FALSE(is_null(obj));
	TEST_ASSERT_TRUE(is_pair(obj));
	has_one_type(obj);
	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));

	TEST_ASSERT_EQUAL(car(obj), a);
	TEST_ASSERT_EQUAL(cdr(obj), b);

	set_car(obj, a2);
	TEST_ASSERT_EQUAL(car(obj), a2);
	TEST_ASSERT_EQUAL(cdr(obj), b);

	set_cdr(obj, b2);
	TEST_ASSERT_EQUAL(car(obj), a2);
	TEST_ASSERT_EQUAL(cdr(obj), b2);
}

void obj_symbol(void)
{
	meta_data meta_data = 126;
	char *id = "Blue sky";
	object obj = from_name(id, meta_data);

	TEST_ASSERT_TRUE(is_symbol(obj));
	has_one_type(obj);

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL_STRING(id, to_name(obj));

	TEST_ASSERT_EQUAL(DOT, from_name(".", NO_META_DATA));
	TEST_ASSERT_EQUAL(QUASIQUOTE, from_name("quasiquote", NO_META_DATA));
	TEST_ASSERT_EQUAL(QUOTE, from_name("quote", NO_META_DATA));
	TEST_ASSERT_EQUAL(UNQUOTE, from_name("unquote", NO_META_DATA));
}

void obj_to_text(void)
{
	TEST_ASSERT_EQUAL_STRING("'()", to_text(EMPTY_LIST));
	TEST_ASSERT_EQUAL_STRING("END-OF-SOURCE", to_text(EOS));

	TEST_ASSERT_EQUAL_STRING("1", to_text(from_integer(1, NO_META_DATA)));
	TEST_ASSERT_EQUAL_STRING(
		"9223372036854775807",
		to_text(from_integer(integer_max, NO_META_DATA)));
	TEST_ASSERT_EQUAL_STRING(
		"-9223372036854775808",
		to_text(from_integer(integer_min, NO_META_DATA)));

	TEST_ASSERT_EQUAL_STRING(
		"1000.0001", to_text(from_floating(1000.0001, NO_META_DATA)));
	TEST_ASSERT_EQUAL_STRING(
		"1.79769313486232e+308",
		to_text(from_floating(floating_max, NO_META_DATA)));
	TEST_ASSERT_EQUAL_STRING(
		"-1.79769313486232e+308",
		to_text(from_floating(floating_min, NO_META_DATA)));

	TEST_ASSERT_EQUAL_STRING(
		"I just ran into him.",
		to_text(from_string("I just ran into him.", NO_META_DATA)));

	TEST_ASSERT_EQUAL_STRING("eval",
				 to_text(from_name("eval", NO_META_DATA)));

	TEST_ASSERT_EQUAL_STRING(
		"(1 2 3)", to_text(cons(from_integer(1, NO_META_DATA),
					cons(from_integer(2, NO_META_DATA),
					     cons(from_integer(3, NO_META_DATA),
						  EMPTY_LIST, NO_META_DATA),
					     NO_META_DATA),
					NO_META_DATA)));

	TEST_ASSERT_EQUAL_STRING(
		"(1 2 . 3)",
		to_text(cons(from_integer(1, NO_META_DATA),
			     cons(from_integer(2, NO_META_DATA),
				  from_integer(3, NO_META_DATA), NO_META_DATA),
			     NO_META_DATA)));
}

int test_object(void)
{
	RUN_TEST(obj_free);
	RUN_TEST(obj_error);
	RUN_TEST(obj_singletons);
	RUN_TEST(obj_integer);
	RUN_TEST(obj_floating);
	RUN_TEST(obj_string);
	RUN_TEST(obj_symbol);
	RUN_TEST(obj_pair);
	RUN_TEST(obj_to_text);
	return 0;
}
