
#include "../unity/src/unity.h"
#include "../src/list.h"

static object one;
static object two;
static object three;
static object four;
static object five;

object end(object lst)
{
	for (; is_pair(lst); lst = cdr(lst))
		;
	return lst;
}

static void set_items(void)
{
	one = of_integer(1, NO_META_DATA);
	two = of_integer(2, NO_META_DATA);
	three = of_integer(3, NO_META_DATA);
	four = of_integer(4, NO_META_DATA);
	five = of_integer(5, NO_META_DATA);
}

void test_listv(void)
{
	TEST_ASSERT_EQUAL(EMPTY_LIST, listv(VA_TERM));

	object lst = listv(one, two, three, four, five, VA_TERM);
	TEST_ASSERT_EQUAL_STRING("(1 2 3 4 5)", to_text(lst));
	TEST_ASSERT_EQUAL(EMPTY_LIST, end(lst));
}

void test_listn(void)
{
	object lst;

	lst = list1(one);
	TEST_ASSERT_EQUAL_STRING("(1)", to_text(lst));
	TEST_ASSERT_EQUAL(EMPTY_LIST, end(lst));

	lst = list2(one, two);
	TEST_ASSERT_EQUAL_STRING("(1 2)", to_text(lst));
	TEST_ASSERT_EQUAL(EMPTY_LIST, end(lst));

	lst = list3(one, two, three);
	TEST_ASSERT_EQUAL_STRING("(1 2 3)", to_text(lst));
	TEST_ASSERT_EQUAL(EMPTY_LIST, end(lst));

	lst = list4(one, two, three, four);
	TEST_ASSERT_EQUAL_STRING("(1 2 3 4)", to_text(lst));
	TEST_ASSERT_EQUAL(EMPTY_LIST, end(lst));
}

void test_list_length(void)
{
	TEST_ASSERT_EQUAL_INT(0, length(EMPTY_LIST));
	TEST_ASSERT_EQUAL_INT(1, length(list1(one)));
	TEST_ASSERT_EQUAL_INT(3, length(list3(one, two, three)));
}

object mult_by_two(object n)
{
	return of_integer(to_integer(n) * 2, NO_META_DATA);
}

void test_list_map(void)
{
	TEST_ASSERT_EQUAL(EMPTY_LIST, map(mult_by_two, EMPTY_LIST));

	object lst = list4(one, two, three, four);
	object mapped = map(mult_by_two, lst);
	TEST_ASSERT_EQUAL_STRING("(2 4 6 8)", to_text(mapped));
	TEST_ASSERT_EQUAL(EMPTY_LIST, end(lst));
}

int test_list(void)
{
	set_items();
	RUN_TEST(test_listv);
	RUN_TEST(test_listn);
	RUN_TEST(test_list_length);
	RUN_TEST(test_list_map);

	return 0;
}
