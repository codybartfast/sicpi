
#include "../unity/src/unity.h"
#include "../src/primitive-procedures.h"

#include "../src/list.h"


static object one;
static object two;
static object three;
static object five;
static object seven;

static void set_items(void)
{
	one = of_integer(1, NO_META_DATA);
	two = of_integer(2, NO_META_DATA);
	three = of_integer(3, NO_META_DATA);
	five = of_integer(5, NO_META_DATA);
	seven = of_integer(7, NO_META_DATA);
}

void test_pp_add(void)
{
	TEST_ASSERT_EQUAL(9, to_integer(Add(list2(two, seven))));
}

int test_primitive_procedures(void)
{
	set_items();
	RUN_TEST(test_pp_add);

	return 0;
}
