#include "../unity/src/unity.h"
#include "test-string-builder.h"

void setUp(void)
{
}

void tearDown(void)
{
}

int main(void)
{
	UNITY_BEGIN();

	test_string_builder();

	return UNITY_END();
}