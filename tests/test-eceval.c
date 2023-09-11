
#include "../unity/src/unity.h"
#include "../src/explicit-control-evaluator.h"

#include "../src/run.c"

object to_expr(char *source){
	return car(load(source_string(source, 0)));
}

void test_eceval_self_evaluating(void)
{
	object expr;

	expr = of_integer(486, NO_META_DATA);
	TEST_ASSERT_EQUAL_STRING("486", to_text(EC_Eval(expr)));

	expr = of_floating(48.11, NO_META_DATA);
	TEST_ASSERT_EQUAL_STRING("48.11", to_text(EC_Eval(expr)));

	expr = of_string("Smelly pants wee!", NO_META_DATA);
	TEST_ASSERT_EQUAL_STRING("Smelly pants wee!", to_text(EC_Eval(expr)));
}

void test_eceval_initial_environment(void)
{
	// object expr;

	// expr = to_expr("true");
	// TEST_ASSERT_EQUAL_STRING("true", to_text(EC_Eval(expr)));
}

int test_eceval(void)
{
	RUN_TEST(test_eceval_self_evaluating);
	RUN_TEST(test_eceval_initial_environment);

	return 0;
}
