#include "../unity/src/unity.h"
#include "../src/token-table.h"

size_t tt_capacity(token_table tt)
{
	return (tt->table_end - tt->table);
}

size_t tt_available(token_table tt)
{
	return tt->table_end - tt->next;
}

void tt_new(void)
{
	size_t initial_capacity = 43;
	token_table tt = token_table_new(initial_capacity);
	TEST_ASSERT_NOT_NULL(tt);
	TEST_ASSERT_NOT_NULL(tt->table);
	TEST_ASSERT_EQUAL_PTR(tt->table, tt->next);
	TEST_ASSERT_EQUAL_size_t(initial_capacity, tt_capacity(tt));

	token_table_free(tt);
}

void tt_new_accept_zero(void)
{
	token_table tt = token_table_new(0);
	TEST_ASSERT_GREATER_THAN_size_t(0, tt_capacity(tt));

	token_table_free(tt);
}

void tt_grow(void)
{
	TOKEN_TABLE_KEY_T key = -1;
	token_table tt = token_table_new(1);
	TEST_ASSERT_EQUAL_UINT(1, tt_capacity(tt));

	key = token_table_add(tt, NULL);
	TEST_ASSERT_EQUAL_UINT64(0, key);
	TEST_ASSERT_EQUAL_UINT(1, tt_capacity(tt));

	key = token_table_add(tt, NULL);
	TEST_ASSERT_EQUAL_UINT64(1, key);
	TEST_ASSERT_EQUAL_UINT(2, tt_capacity(tt));

	key = token_table_add(tt, NULL);
	TEST_ASSERT_EQUAL_UINT64(2, key);
	TEST_ASSERT_EQUAL_UINT(4, tt_capacity(tt));

	key = token_table_add(tt, NULL);
	TEST_ASSERT_EQUAL_UINT64(3, key);
	TEST_ASSERT_EQUAL_UINT(4, tt_capacity(tt));

	key = token_table_add(tt, NULL);
	TEST_ASSERT_EQUAL_UINT64(4, key);
	TEST_ASSERT_EQUAL_UINT(8, tt_capacity(tt));

	token_table_free(tt);
}

void test_token_table(void)
{
	RUN_TEST(tt_new);
	RUN_TEST(tt_new_accept_zero);
	RUN_TEST(tt_grow);
}
