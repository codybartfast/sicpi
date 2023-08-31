#include "../src/obarray.h"
#include "../unity/src/unity.h"

#include "../src/object.h"

#include <stdbool.h>
#include <stdio.h>

static size_t capacity(obarray x)
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

void test_obarray_intern(void)
{
	struct obarray obarray;
	obarray_init(&obarray, 0);

	char *apple = "apple";
	char *apple2 = "apple";
	char *banana = "banana";

	object apple_i = obarray_intern(&obarray, apple);
	TEST_ASSERT_FALSE(apple == to_name(apple_i));
	TEST_ASSERT_EQUAL_STRING(apple, to_name(apple_i));

	object banana_i = obarray_intern(&obarray, banana);
	TEST_ASSERT_FALSE(banana == to_name(banana_i));
	TEST_ASSERT_EQUAL_STRING(banana, to_name(banana_i));

	object apple2_i = obarray_intern(&obarray, apple2);
	TEST_ASSERT_EQUAL_PTR(apple2_i, apple_i);
	TEST_ASSERT_EQUAL_STRING(apple2, to_name(apple2_i));
}

void test_obarray_grow(void)
{
	struct obarray oba;
	obarray_init(&oba, 1);

	TEST_ASSERT_EQUAL(1, capacity(&oba));

	char *zero = "0";
	char *one = "1";
	char *ten = "10";
	char *hundred = "100";
	char *thousand = "1000";

	char numstr[5];
	for (int i = 0; i < 1050; i++) {
		sprintf(numstr, "%d", i);
		obarray_intern(&oba, numstr);
	}
	TEST_ASSERT_EQUAL(2048, capacity(&oba));

	TEST_ASSERT_EQUAL_STRING(zero, to_name(*oba.start));
	TEST_ASSERT_EQUAL_STRING(one, to_name(*(oba.start + 1)));
	TEST_ASSERT_EQUAL_STRING(ten, to_name(*(oba.start + 10)));
	TEST_ASSERT_EQUAL_STRING(hundred, to_name(*(oba.start + 100)));
	TEST_ASSERT_EQUAL_STRING(thousand, to_name(*(oba.start + 1000)));

	TEST_ASSERT_EQUAL_PTR(*(oba.start + 100), obarray_intern(&oba, "100"));
}

void test_obarray()
{
	RUN_TEST(test_obarray_init);
	RUN_TEST(test_obarray_new);
	RUN_TEST(test_obarray_intern);
	RUN_TEST(test_obarray_grow);
}
