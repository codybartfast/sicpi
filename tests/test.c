#include "../unity/src/unity.h"

#include "test-source.h"
#include "test-string-builder.h"
#include "test-token-table.h"

void setUp(void)
{
}

void tearDown(void)
{
}

int main(void)
{
	UNITY_BEGIN();

	test_source();
	test_string_builder();
	test_token_table();

	return UNITY_END();
}
