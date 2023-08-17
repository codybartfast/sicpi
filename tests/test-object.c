#include <stdbool.h>
#include "../unity/src/unity.h"

#include "../src/object.h"

bool has_one_type(object obj)
{
	int count = 0;

	if (is_number(obj))
		count++;

	return 1 == count;
}

void obj_free(void)
{
	object_free(from_integer(0, NO_META_DATA));
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

int test_object(void)
{
	RUN_TEST(obj_free);
	RUN_TEST(obj_new_integer);
	return 0;
}
