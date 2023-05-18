#include "../unity/src/unity.h"
#include "../src/string-builder.h"

size_t sb_capacity(string_builder sb)
{
	return (sb->buff_end - sb->buff);
}

size_t sb_available(string_builder sb)
{
	return sb->buff_end - sb->next;
}

void sb_new(void)
{
	size_t initial_capacity = 43;
	string_builder sb = new_string_builder(initial_capacity);
	TEST_ASSERT_NOT_NULL(sb);
	TEST_ASSERT_NOT_NULL(sb->buff);
	TEST_ASSERT_EQUAL_PTR(sb->buff, sb->next);
	TEST_ASSERT_EQUAL_size_t(initial_capacity, sb_capacity(sb));

	sb_free(sb);
}

void sb_new_accept_zero(void)
{
	string_builder sb = new_string_builder(0);
	TEST_ASSERT_GREATER_THAN_size_t(0, sb_capacity(sb));

	sb_free(sb);
}

void sb_addc_no_grow(void)
{
	string_builder sb = new_string_builder(4);
	*sb->buff = 'x';
	sb_addc(sb, 'c');
	sb_addc(sb, 'a');
	sb_addc(sb, 't');
	TEST_ASSERT_EQUAL_STRING("cat", sb_current(sb));

	sb_free(sb);
}

void sb_addc_grow(void)
{
	string_builder sb = new_string_builder(1);
	sb_addc(sb, 'c');
	sb_addc(sb, 'a');
	sb_addc(sb, 't');
	sb_addc(sb, 's');
	TEST_ASSERT_EQUAL_STRING("cats", sb_current(sb));
	TEST_ASSERT_GREATER_OR_EQUAL(0, sb_available(sb));
	TEST_ASSERT_EQUAL_STRING("cats", sb_current(sb));

	sb_free(sb);
}

void sb_add_string(void)
{
	string_builder sb = new_string_builder(1);
	sb_adds(sb, "cats");
	sb_adds(sb, " and ");
	sb_adds(sb, "dogs");
	TEST_ASSERT_EQUAL_STRING("cats and dogs", sb_current(sb));

	sb_free(sb);
}

void sb_copy_string(void)
{
	string_builder sb = new_string_builder(0);
	sb_adds(sb, "pepper");
	char *current = sb_current(sb);
	char *copy = sb_copy(sb);
	TEST_ASSERT_EQUAL_STRING("pepper", current);
	TEST_ASSERT_EQUAL_STRING(current, copy);
	if (copy == current) {
		TEST_FAIL();
	}

	sb_free(sb);
}

void sb_copy_string_with_growth(void)
{
	// size tests depend on implemation using growth factor of 2
	string_builder sb = new_string_builder(1);

	TEST_ASSERT_EQUAL_STRING("", sb_copy(sb));
	TEST_ASSERT_EQUAL_size_t(1, sb_capacity(sb));

	sb_addc(sb, '1');
	TEST_ASSERT_EQUAL_STRING("1", sb_copy(sb));
	TEST_ASSERT_EQUAL_size_t(2, sb_capacity(sb));

	sb_addc(sb, '2');
	TEST_ASSERT_EQUAL_STRING("12", sb_copy(sb));
	TEST_ASSERT_EQUAL_size_t(4, sb_capacity(sb));

	sb_adds(sb, "3");
	TEST_ASSERT_EQUAL_STRING("123", sb_copy(sb));
	TEST_ASSERT_EQUAL_size_t(4, sb_capacity(sb));

	sb_addc(sb, '4');
	TEST_ASSERT_EQUAL_STRING("1234", sb_copy(sb));
	TEST_ASSERT_EQUAL_size_t(8, sb_capacity(sb));

	sb_adds(sb, "5");
	TEST_ASSERT_EQUAL_STRING("12345", sb_copy(sb));
	TEST_ASSERT_EQUAL_size_t(8, sb_capacity(sb));

	sb_addc(sb, '6');
	TEST_ASSERT_EQUAL_STRING("123456", sb_copy(sb));
	TEST_ASSERT_EQUAL_size_t(8, sb_capacity(sb));

	sb_adds(sb, "7");
	TEST_ASSERT_EQUAL_STRING("1234567", sb_copy(sb));
	TEST_ASSERT_EQUAL_size_t(8, sb_capacity(sb));

	sb_addc(sb, '8');
	TEST_ASSERT_EQUAL_STRING("12345678", sb_copy(sb));
	TEST_ASSERT_EQUAL_size_t(16, sb_capacity(sb));

	sb_free(sb);
}

void sb_clear_resets_and_retains_capacity(void)
{
	string_builder sb = new_string_builder(1);

	sb_adds(sb, "R");
	TEST_ASSERT_EQUAL_STRING("R", sb_copy(sb));
	size_t small_cap = sb_capacity(sb);

	sb_adds(sb, "aspberry");
	TEST_ASSERT_EQUAL_STRING("Raspberry", sb_copy(sb));
	size_t larger_cap = sb_capacity(sb);
	TEST_ASSERT_GREATER_THAN_size_t(small_cap, larger_cap);

	sb_clear(sb);

	sb_adds(sb, "p");
	TEST_ASSERT_EQUAL_STRING("p", sb_copy(sb));
	TEST_ASSERT_EQUAL_size_t(larger_cap, sb_capacity(sb));

	sb_free(sb);
}

void sb_free_frees(void)
{
	string_builder sb = new_string_builder(1);
	char *next = sb->next;
	TEST_ASSERT_NOT_NULL(sb->next);

	sb_free(sb);

	if (sb->next == next) {
		TEST_FAIL_MESSAGE("expected sb->next to be null or reassigned");
	}
}

int test_string_builder(void)
{
	RUN_TEST(sb_new);
	RUN_TEST(sb_new_accept_zero);
	RUN_TEST(sb_addc_no_grow);
	RUN_TEST(sb_addc_grow);
	RUN_TEST(sb_add_string);
	RUN_TEST(sb_copy_string);
	RUN_TEST(sb_copy_string_with_growth);
	RUN_TEST(sb_clear_resets_and_retains_capacity);
	RUN_TEST(sb_free_frees);
}
