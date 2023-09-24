
#include "../unity/src/unity.h"
#include "../src/explicit-control-evaluator.h"

#include "../src/metacircular-procedures.h"
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
	TEST_ASSERT_NOT_EQUAL(TRUE_VALUE, expr);
	TEST_ASSERT_EQUAL(TRUE_VALUE, EC_Eval(expr));
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

	expr = to_expr("(- 1000 334)");
	TEST_ASSERT_EQUAL(666, to_integer(EC_Eval(expr)));

	expr = to_expr("(/ 10 5)");
	TEST_ASSERT_EQUAL(2, to_integer(EC_Eval(expr)));

	expr = to_expr("(+ 2.7 10)");
	TEST_ASSERT_EQUAL(12.7, to_floating(EC_Eval(expr)));

	expr = to_expr("(+ 21 35 12 7)");
	TEST_ASSERT_EQUAL(75, to_integer(EC_Eval(expr)));

	expr = to_expr("(* 25 4 12)");
	TEST_ASSERT_EQUAL(1200, to_integer(EC_Eval(expr)));

	expr = to_expr("(+ (* 3 5) (- 10 6))");
	TEST_ASSERT_EQUAL(19, to_integer(EC_Eval(expr)));

	expr = to_expr("(+ (* 3 (+ (* 2 4) (+ 3 5))) (+ (- 10 7) 6))");
	TEST_ASSERT_EQUAL(57, to_integer(EC_Eval(expr)));

	expr = to_expr("(+ (* 3"
		       "      (+ (* 2 4)"
		       "         (+ 3 5)))"
		       "   (+ (- 10 7)"
		       "      6))");
	TEST_ASSERT_EQUAL(57, to_integer(EC_Eval(expr)));
}

void test_eceval_define(void)
{
	object expr;
	object env = setup_environment();

	expr = to_expr("(define e (+ 1 1.71828))");
	ec_eval(expr, env);
	TEST_ASSERT_EQUAL_DOUBLE(2.71828,
				 to_floating(lookup_variable_value(
					 of_name("e", NO_META_DATA), env)));

	expr = to_expr("(begin (define (add2 x) (+ 2 x)) (add2 2))");
	TEST_ASSERT_EQUAL_STRING("4", to_text(EC_Eval(expr)));
}

void test_eceval_sequence(void)
{
	object expr;

	expr = to_expr("(begin"
		       "    (define a 1)"
		       "    (define b 1.5)"
		       "    (define c 0.21)"
		       "    (define d 0.00828)"
		       "    (define e (+ a b c d))"
		       "    e"
		       ")");

	TEST_ASSERT_EQUAL_DOUBLE(2.71828, to_floating(EC_Eval(expr)));
}

void test_eceval_conditionals(void)
{
	object expr;

	expr = to_expr("(if true 'apple)");
	TEST_ASSERT_EQUAL_STRING("apple", to_text(EC_Eval(expr)));

	expr = to_expr("(if false 'apple 'pear)");
	TEST_ASSERT_EQUAL_STRING("pear", to_text(EC_Eval(expr)));

	expr = to_expr("(if false 'apple)");
	TEST_ASSERT_EQUAL_STRING("#f", to_text(EC_Eval(expr)));

	expr = to_expr("(if (< 2 3) 'apple)");
	TEST_ASSERT_EQUAL_STRING("apple", to_text(EC_Eval(expr)));

	expr = to_expr("(if (= 4 5) 'apple 'pear)");
	TEST_ASSERT_EQUAL_STRING("pear", to_text(EC_Eval(expr)));

	expr = to_expr("(if (> 4 5) 'apple)");
	TEST_ASSERT_EQUAL_STRING("#f", to_text(EC_Eval(expr)));

	// cond

	expr = to_expr("(cond)");
	TEST_ASSERT_EQUAL_STRING("#f", to_text(EC_Eval(expr)));

	expr = to_expr("(cond (else))");
	TEST_ASSERT_EQUAL_STRING("#void", to_text(EC_Eval(expr)));

	expr = to_expr("(cond (true))");
	TEST_ASSERT_EQUAL_STRING("#void", to_text(EC_Eval(expr)));

	expr = to_expr("(cond (false))");
	TEST_ASSERT_EQUAL_STRING("#f", to_text(EC_Eval(expr)));

	expr = to_expr("(cond ((= 4 5) 'apple) (false 'banana) (else 'end))");
	TEST_ASSERT_EQUAL_STRING("end", to_text(EC_Eval(expr)));

	expr = to_expr("(cond (true 'apple) (false 'banana) (else 'end))");
	TEST_ASSERT_EQUAL_STRING("apple", to_text(EC_Eval(expr)));

	expr = to_expr("(cond (false 'apple) (true 'banana) (else 'end))");
	TEST_ASSERT_EQUAL_STRING("banana", to_text(EC_Eval(expr)));

	expr = to_expr("(cond (true 'apple) (true 'banana) (else 'end))");
	TEST_ASSERT_EQUAL_STRING("apple", to_text(EC_Eval(expr)));
}

void test_eceval_primitive_procedures(void)
{
	// We're not checking the implementation of these functions (that's
	// elsewhere), just that they're defined.

	object expr;

	// Arithmetic

	expr = to_expr("(+ 2 2)");
	TEST_ASSERT_EQUAL_STRING("4", to_text(EC_Eval(expr)));

	expr = to_expr("(- 4 2)");
	TEST_ASSERT_EQUAL_STRING("2", to_text(EC_Eval(expr)));

	expr = to_expr("(* 4 2)");
	TEST_ASSERT_EQUAL_STRING("8", to_text(EC_Eval(expr)));

	expr = to_expr("(/ 9 2)");
	TEST_ASSERT_EQUAL_STRING("4.5", to_text(EC_Eval(expr)));

	// Comparison

	expr = to_expr("(> 3 2)");
	TEST_ASSERT_EQUAL_STRING("#t", to_text(EC_Eval(expr)));

	expr = to_expr("(< 3 2)");
	TEST_ASSERT_EQUAL_STRING("#f", to_text(EC_Eval(expr)));

	expr = to_expr("(= 2 3)");
	TEST_ASSERT_EQUAL_STRING("#f", to_text(EC_Eval(expr)));

	// Logic

	expr = to_expr("(and true false)");
	TEST_ASSERT_EQUAL_STRING("#f", to_text(EC_Eval(expr)));

	expr = to_expr("(or true false)");
	TEST_ASSERT_EQUAL_STRING("#t", to_text(EC_Eval(expr)));

	expr = to_expr("(not false)");
	TEST_ASSERT_EQUAL_STRING("#t", to_text(EC_Eval(expr)));
}

int test_explicit_control_evaluator(void)
{
	RUN_TEST(test_eceval_self_evaluating);
	RUN_TEST(test_eceval_variable);
	RUN_TEST(test_eceval_quoted);
	RUN_TEST(test_eceval_section_1_1_1);
	RUN_TEST(test_eceval_define);
	RUN_TEST(test_eceval_sequence);
	RUN_TEST(test_eceval_conditionals);
	RUN_TEST(test_eceval_primitive_procedures);

	return 0;
}
