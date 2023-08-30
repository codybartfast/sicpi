#include "../src/obarray.h"
#include "../unity/src/unity.h"

#include <stdbool.h>

size_t capacity(obarray x)
{
	return (x->end - x->start);
}

// size_t tt_available(token_table tt)
// {
// 	return tt->table_end - tt->next;
// }

void test_obarray_init(void)
{
	struct obarray oba;
	size_t initial_capacity = 43;
	obarray_init(&oba, initial_capacity);
	// TEST_ASSERT_NOT_NULL(tt);
	TEST_ASSERT_NOT_NULL(oba.start);
	TEST_ASSERT_EQUAL_PTR(oba.start, oba.next);
	TEST_ASSERT_EQUAL_size_t(initial_capacity, capacity(&oba));

	obarray_free_members(&oba);
	TEST_ASSERT_NULL(oba.start);
	TEST_ASSERT_NULL(oba.next);
	TEST_ASSERT_NULL(oba.end);
}

void test_obarray_new(void)
{
	size_t initial_capacity = 43;
	obarray oba = obarray_new(initial_capacity);
	TEST_ASSERT_NOT_NULL(oba);
	TEST_ASSERT_NOT_NULL(oba->start);
	TEST_ASSERT_EQUAL_PTR(oba->start, oba->next);

	void *startptr = oba->start;

	obarray_free(oba);
	TEST_ASSERT_FALSE(startptr == oba->start);
}

void test_obarray()
{
	RUN_TEST(test_obarray_init);
	RUN_TEST(test_obarray_new);
}
