
#include "../unity/src/unity.h"

#include "../src/explicit-control-evaluator.h"

void test_eceval_self_evaluating(void)
{
	object expr;

	expr = of_integer(486, NO_META_DATA);
	TEST_ASSERT_EQUAL_STRING("486", to_text(ec_eval(expr)));

	expr = of_floating(48.11, NO_META_DATA);
	TEST_ASSERT_EQUAL_STRING("48.11", to_text(ec_eval(expr)));

	expr = of_string("Smelly pants wee!", NO_META_DATA);
	TEST_ASSERT_EQUAL_STRING("Smelly pants wee!", to_text(ec_eval(expr)));
}

int test_eceval(void)
{
	RUN_TEST(test_eceval_self_evaluating);

	return 0;
}
