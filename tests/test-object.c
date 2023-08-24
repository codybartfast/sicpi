#include "../src/object.h"
#include "../unity/src/unity.h"

#include <stdbool.h>

bool has_one_type(object obj)
{
	int count = 0;

	if (is_error(obj))
		count++;
	else if (is_number(obj))
		count++;
	else if (is_string(obj))
		count++;

	return 1 == count;
}

void obj_free(void)
{
	object_free(from_integer(0, NO_META_DATA));
}

//
// Error
// =============================================================================
//

void obj_new_error(void)
{
	meta_data meta_data = 125;
	enum error_kind error_kind = ERROR_LEXER;
	object obj = from_error_kind(error_kind, meta_data);

	TEST_ASSERT_TRUE(is_error(obj));
	TEST_ASSERT_TRUE(has_one_type(obj));

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL(error_kind, to_error_kind(obj));
}

//
// Number
// =============================================================================
//

void obj_new_integer(void)
{
	meta_data meta_data = 124;
	integer integer = 486;
	object obj = from_integer(integer, meta_data);

	TEST_ASSERT_TRUE(is_number(obj));
	TEST_ASSERT_TRUE(has_one_type(obj));

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL(integer, to_integer(obj));
}

void obj_new_floating(void)
{
	meta_data meta_data = 124;
	floating floating = 48.6;
	object obj = from_floating(floating, meta_data);

	TEST_ASSERT_TRUE(is_number(obj));
	TEST_ASSERT_TRUE(has_one_type(obj));

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL(floating, to_floating(obj));
}

void obj_new_string(void)
{
	meta_data meta_data = 125;
	char *string = "Blue dog";
	object obj = from_string(string, meta_data);

	TEST_ASSERT_TRUE(is_string(obj));
	TEST_ASSERT_TRUE(has_one_type(obj));

	TEST_ASSERT_EQUAL(meta_data, object_meta_data(obj));
	TEST_ASSERT_EQUAL(string, to_string(obj));
}

int test_object(void)
{
	RUN_TEST(obj_free);
	RUN_TEST(obj_new_error);
	RUN_TEST(obj_new_integer);
	RUN_TEST(obj_new_floating);
	RUN_TEST(obj_new_string);
	return 0;
}
