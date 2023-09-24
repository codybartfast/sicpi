
#include "../unity/src/unity.h"
#include "../src/primitive-procedures.h"

#include "../src/list.h"

static object zero;
static object one;
static object two;
static object three;
static object five;
static object seven;
static object one05;

static object one_f;
static object two_f;
static object three_f;
static object five_f;
static object seven_1_f;
static object one05_f;

static void set_items(void)
{
	zero = of_integer(0, NO_META_DATA);
	one = of_integer(1, NO_META_DATA);
	two = of_integer(2, NO_META_DATA);
	three = of_integer(3, NO_META_DATA);
	five = of_integer(5, NO_META_DATA);
	seven = of_integer(7, NO_META_DATA);
	one05 = of_integer(105, NO_META_DATA);

	one_f = of_floating(1.0, NO_META_DATA);
	two_f = of_floating(2.0, NO_META_DATA);
	three_f = of_floating(3.0, NO_META_DATA);
	five_f = of_floating(5.0, NO_META_DATA);
	seven_1_f = of_floating(7.1, NO_META_DATA);
	one05_f = of_floating(105, NO_META_DATA);
}

void test_pp_add(void)
{
	TEST_ASSERT_EQUAL(0, to_integer(Add(EMPTY_LIST)));
	TEST_ASSERT_EQUAL(7, to_integer(Add(list1(seven))));
	TEST_ASSERT_EQUAL(9, to_integer(Add(list2(two, seven))));
	TEST_ASSERT_EQUAL(14, to_integer(Add(list3(two, five, seven))));

	TEST_ASSERT_EQUAL_DOUBLE(7.1, to_floating(Add(list1(seven_1_f))));
	TEST_ASSERT_EQUAL_DOUBLE(9.1,
				 to_floating(Add(list2(two_f, seven_1_f))));
	TEST_ASSERT_EQUAL_DOUBLE(
		14.1, to_floating(Add(list3(two_f, five_f, seven_1_f))));
	TEST_ASSERT_EQUAL_DOUBLE(14.,
				 to_floating(Add(list3(two, five_f, seven))));
}

void test_pp_sub(void)
{
	object rslt;
	rslt = Sub(EMPTY_LIST);
	TEST_ASSERT_TRUE(is_error(rslt));
	TEST_ASSERT_EQUAL(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
			  to_error_kind(rslt));

	TEST_ASSERT_EQUAL(-7, to_integer(Sub(list1(seven))));
	TEST_ASSERT_EQUAL(-5, to_integer(Sub(list2(two, seven))));
	TEST_ASSERT_EQUAL(-10, to_integer(Sub(list3(two, five, seven))));

	TEST_ASSERT_EQUAL_DOUBLE(-7.1, to_floating(Sub(list1(seven_1_f))));
	TEST_ASSERT_EQUAL_DOUBLE(-5.1,
				 to_floating(Sub(list2(two_f, seven_1_f))));
	TEST_ASSERT_EQUAL_DOUBLE(
		-10.1, to_floating(Sub(list3(two_f, five, seven_1_f))));
}

void test_pp_mul(void)
{
	TEST_ASSERT_EQUAL(1, to_integer(Mul(EMPTY_LIST)));
	TEST_ASSERT_EQUAL(7, to_integer(Mul(list1(seven))));
	TEST_ASSERT_EQUAL(14, to_integer(Mul(list2(two, seven))));
	TEST_ASSERT_EQUAL(70, to_integer(Mul(list3(two, five, seven))));

	TEST_ASSERT_EQUAL_DOUBLE(7.1, to_floating(Mul(list1(seven_1_f))));
	TEST_ASSERT_EQUAL_DOUBLE(14.2,
				 to_floating(Mul(list2(two_f, seven_1_f))));
	TEST_ASSERT_EQUAL_DOUBLE(
		71.0, to_floating(Mul(list3(two_f, five_f, seven_1_f))));
	TEST_ASSERT_EQUAL_DOUBLE(70.0,
				 to_floating(Mul(list3(two, five_f, seven))));
}

void test_pp_div(void)
{
	object rslt;

	rslt = Div(EMPTY_LIST);
	TEST_ASSERT_TRUE(is_error(rslt));
	TEST_ASSERT_EQUAL(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,
			  to_error_kind(rslt));

	TEST_ASSERT_DOUBLE_WITHIN(0.000000001, 0.142857143,
				  to_floating(Div(list1(seven))));
	TEST_ASSERT_DOUBLE_WITHIN(0.000000001, 0.285714286,
				  to_floating(Div(list2(two, seven))));
	TEST_ASSERT_DOUBLE_WITHIN(0.000000001, 0.0571428571,
				  to_floating(Div(list3(two, five, seven))));

	rslt = Div(list4(five, three, zero, two));
	TEST_ASSERT_TRUE(is_error(rslt));
	TEST_ASSERT_EQUAL(ERROR_DIVISION_BY_ZERO, to_error_kind(rslt));

	rslt = Div(list2(one, zero));
	TEST_ASSERT_TRUE(is_error(rslt));
	TEST_ASSERT_EQUAL(ERROR_DIVISION_BY_ZERO, to_error_kind(rslt));

	// Optional
	TEST_ASSERT_EQUAL(3, to_integer(Div(list3(one05, five, seven))));
	TEST_ASSERT_EQUAL(3, to_floating(Div(list3(one05_f, five, seven))));
	TEST_ASSERT_EQUAL(3, to_floating(Div(list3(one05, five_f, seven))));
}

void test_pp_comparison(void)
{
	// Greater Than
	TEST_ASSERT_TRUE(to_bool(Greater_Than(list2(two, one))));
	TEST_ASSERT_TRUE(to_bool(Greater_Than(list4(five, three, two, one))));
	TEST_ASSERT_TRUE(to_bool(Greater_Than(list4(five, three_f, two, one))));

	TEST_ASSERT_FALSE(to_bool(Greater_Than(list2(two, two))));
	TEST_ASSERT_FALSE(to_bool(Greater_Than(list4(five, three, one, two))));
	TEST_ASSERT_FALSE(
		to_bool(Greater_Than(list4(five, three, one, one_f))));

	// Less Than
	TEST_ASSERT_TRUE(to_bool(Less_Than(list4(one, two, three, five))));
	TEST_ASSERT_TRUE(to_bool(Less_Than(list2(one, two))));
	TEST_ASSERT_TRUE(to_bool(Less_Than(list4(one, two, three_f, five))));

	TEST_ASSERT_FALSE(to_bool(Less_Than(list2(two, two))));
	TEST_ASSERT_FALSE(to_bool(Less_Than(list4(one, two, five, three))));
	TEST_ASSERT_FALSE(to_bool(Less_Than(list4(one, two, three, three_f))));
}

int test_primitive_procedures(void)
{
	set_items();
	RUN_TEST(test_pp_add);
	RUN_TEST(test_pp_sub);
	RUN_TEST(test_pp_mul);
	RUN_TEST(test_pp_div);
	RUN_TEST(test_pp_comparison);

	return 0;
}
