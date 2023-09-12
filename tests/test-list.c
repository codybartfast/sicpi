
#include "../unity/src/unity.h"
#include "../src/list.h"

object one;
object two;
object three;
object four;
object five;

object end(object lst)
{
	for (; is_pair(lst); lst = cdr(lst))
		;
	return lst;
}

void set_items(void)
{
	one = of_integer(1, NO_META_DATA);
	two = of_integer(2, NO_META_DATA);
	three = of_integer(3, NO_META_DATA);
	four = of_integer(4, NO_META_DATA);
	five = of_integer(5, NO_META_DATA);
}

void test_listv(void)
{
	TEST_ASSERT_EQUAL(EMPTY_LIST, listv(EMPTY_LIST));

	object lst = listv(one, two, three, four, five, EMPTY_LIST);
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


int test_list(void)
{
	set_items();
	RUN_TEST(test_listv);
	RUN_TEST(test_listn);

	return 0;
}
