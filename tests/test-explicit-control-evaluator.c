
#include "../unity/src/unity.h"
#include "../src/explicit-control-evaluator.h"

#include "../src/run.c"

object to_expr(char *source)
{
	return car(load(source_string(source, 0)));
}

void test_eceval_self_evaluating(void)
{
	object expr;

	expr = of_floating(48.11, NO_META_DATA);
	TEST_ASSERT_EQUAL_STRING("48.11", to_text(EC_Eval(expr)));

	expr = of_string("Smelly pants wee!", NO_META_DATA);
	TEST_ASSERT_EQUAL_STRING("Smelly pants wee!", to_text(EC_Eval(expr)));
}

void test_eceval_variable(void)
{
	object expr = to_expr("true");
	TEST_ASSERT_NOT_EQUAL(TRUE, expr);
	TEST_ASSERT_EQUAL(TRUE, EC_Eval(expr));
}

void test_eceval_quoted(void)
{
	object expr, symb;

	expr = to_expr("'apple");
	symb = of_name("apple", NO_META_DATA);

	TEST_ASSERT_NOT_EQUAL(symb, expr);
	TEST_ASSERT_EQUAL(symb, EC_Eval(expr));
}

void test_eceval_section_1_1_1(void)
{
	object expr;

	expr = of_integer(486, NO_META_DATA);
	TEST_ASSERT_EQUAL_STRING("486", to_text(EC_Eval(expr)));

	expr = to_expr("(+ 137 349)");
	TEST_ASSERT_EQUAL(486, to_integer(EC_Eval(expr)));
}

int test_eceval(void)
{
	RUN_TEST(test_eceval_self_evaluating);
	RUN_TEST(test_eceval_variable);
	RUN_TEST(test_eceval_quoted);
	RUN_TEST(test_eceval_section_1_1_1);

	return 0;
}