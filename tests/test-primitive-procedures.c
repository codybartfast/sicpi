
#include "../unity/src/unity.h"
#include "../src/primitive-procedures.h"

#include "../src/list.h"

static object one;
static object two;
static object three;
static object five;
static object seven;

static object one_f;
static object two_f;
static object three_f;
static object five_f;
static object seven_f;

static void set_items(void)
{
	one = of_integer(1, NO_META_DATA);
	two = of_integer(2, NO_META_DATA);
	three = of_integer(3, NO_META_DATA);
	five = of_integer(5, NO_META_DATA);
	seven = of_integer(7, NO_META_DATA);

	one_f = of_floating(1.0, NO_META_DATA);
	two_f = of_floating(2.0, NO_META_DATA);
	three_f = of_floating(3.0, NO_META_DATA);
	five_f = of_floating(5.0, NO_META_DATA);
	seven_f = of_floating(7.0, NO_META_DATA);
}

void test_pp_add(void)
{
	TEST_ASSERT_EQUAL(0, to_integer(Add(EMPTY_LIST)));
	TEST_ASSERT_EQUAL(7, to_integer(Add(list1(seven))));
	TEST_ASSERT_EQUAL(9, to_integer(Add(list2(two, seven))));
	TEST_ASSERT_EQUAL(14, to_integer(Add(list3(two, five, seven))));

	TEST_ASSERT_EQUAL(7.0, to_floating(Add(list1(seven_f))));
	TEST_ASSERT_EQUAL(9.0, to_floating(Add(list2(two_f, seven_f))));
	TEST_ASSERT_EQUAL(14.0,
			  to_floating(Add(list3(two_f, five_f, seven_f))));
	TEST_ASSERT_EQUAL(14.0, to_floating(Add(list3(two, five_f, seven))));
}

void test_pp_sub(void)
{
	object rslt = rslt = Sub(EMPTY_LIST);
	TEST_ASSERT_TRUE(is_error(rslt));
	TEST_ASSERT_EQUAL(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
			  to_error_kind(rslt));

	TEST_ASSERT_EQUAL(-7, to_integer(Sub(list1(seven))));
	TEST_ASSERT_EQUAL(-5, to_integer(Sub(list2(two, seven))));
	TEST_ASSERT_EQUAL(-10, to_integer(Sub(list3(two, five, seven))));

	TEST_ASSERT_EQUAL(-7, to_floating(Sub(list1(seven_f))));
	TEST_ASSERT_EQUAL(-5, to_floating(Sub(list2(two_f, seven_f))));
	TEST_ASSERT_EQUAL(-10, to_floating(Sub(list3(two_f, five, seven_f))));
}

int test_primitive_procedures(void)
{
	set_items();
	RUN_TEST(test_pp_add);
	RUN_TEST(test_pp_sub);

	return 0;
}
