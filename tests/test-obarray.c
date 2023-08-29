#include "../src/obarray.h"
#include "../unity/src/unity.h"

#include <stdbool.h>

void test_obarray_some_test(void)
{
	TEST_FAIL_MESSAGE("hello");
}

void test_obarray()
{
	RUN_TEST(test_obarray_some_test);
}
