#include "../unity/src/unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_string_builder(void);
void test_source(void);
void test_token_table(void);
void test_lexer(void);
void test_obarray(void);
void test_object(void);
void test_parser(void);
void test_eceval(void);

int main(void)
{
	UNITY_BEGIN();

	test_string_builder();
	test_source();
	test_token_table();
	test_lexer();
	test_obarray();
	test_object();
	test_parser();

	test_eceval();

	return UNITY_END();
}
