#include "../src/object.h"
#include "../unity/src/unity.h"

#include <stdbool.h>

bool has_one_type(object obj)
{
	return is_error(obj) + is_symbol(obj) + is_number(obj) +
	       is_string(obj) + is_pair(obj);
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
	TEST_ASSERT_TRUE(has_one_type(obj));

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL(error_kind, to_error_kind(obj));
}

void obj_singletons(void)
{
	is_singleton(Eos);
	has_one_type(Eos);

	is_singleton(Empty_List);
	has_one_type(Empty_List);
	TEST_ASSERT_NOT_EQUAL(Eos, Empty_List);
}

void obj_integer(void)
{
	meta_data meta_data = 124;
	integer integer = 486;
	object obj = from_integer(integer, meta_data);

	TEST_ASSERT_TRUE(is_number(obj));
	TEST_ASSERT_TRUE(has_one_type(obj));

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL(integer, to_integer(obj));
}

void obj_floating(void)
{
	meta_data meta_data = 124;
	floating floating = 48.6;
	object obj = from_floating(floating, meta_data);

	TEST_ASSERT_TRUE(is_number(obj));
	TEST_ASSERT_TRUE(has_one_type(obj));

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL(floating, to_floating(obj));
}

void obj_string(void)
{
	meta_data meta_data = 125;
	char *string = "Blue dog";
	object obj = from_string(string, meta_data);

	TEST_ASSERT_TRUE(is_string(obj));
	TEST_ASSERT_TRUE(has_one_type(obj));

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL(string, to_string(obj));
}

void obj_symbol(void)
{
	meta_data meta_data = 126;
	char *id = "Blue sky";
	object obj = from_name(id, meta_data);

	TEST_ASSERT_TRUE(is_symbol(obj));
	TEST_ASSERT_TRUE(has_one_type(obj));

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL(id, to_name(obj));
}

void obj_pair(void)
{
	meta_data meta_data = 127;
	object a = from_integer(39, NO_META_DATA);
	object b = from_string("thrity nine", NO_META_DATA);
	object obj = cons(a, b, meta_data);

	TEST_ASSERT_TRUE(is_pair(obj));
	TEST_ASSERT_TRUE(has_one_type(obj));
	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));

	TEST_ASSERT_EQUAL(car(obj), a);
	TEST_ASSERT_EQUAL(cdr(obj), b);
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
	return 0;
}
