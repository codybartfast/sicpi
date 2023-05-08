#include "../unity/src/unity.h"
#include "../src/version.h"


void setUp(void) { }

void tearDown(void) { }

void test_function_should_doBlahAndBlah(void) {
//     TEST_PASS();
}

void test_function_should_doAlsoDoBlah(void) {
//     TEST_FAIL();
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_function_should_doBlahAndBlah);
    RUN_TEST(test_function_should_doAlsoDoBlah);
    return UNITY_END();
}