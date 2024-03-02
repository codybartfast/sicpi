#include "../unity/src/unity.h"
#include "../src/token-table.h"

#define NMD 0 // No Meta Data

static size_t capacity(token_table tt)
{
	return (tt->table_end - tt->table);
}

void tt_new_empty(void)
{
	size_t initial_capacity = 43;
	token_table tt = token_table_new_empty(initial_capacity);
	TEST_ASSERT_NOT_NULL(tt);
	TEST_ASSERT_NOT_NULL(tt->table);
	TEST_ASSERT_EQUAL_PTR(tt->table, tt->next);
	TEST_ASSERT_EQUAL_size_t(initial_capacity, capacity(tt));

	token_table_free(tt);
}

void tt_new_empty_accept_zero_does_not_add_nometadata_token(void)
{
	token_table tt = token_table_new_empty(0);
	TEST_ASSERT_GREATER_THAN_size_t(0, capacity(tt));
	token rslt = token_table_get(tt, NMD);
	TEST_ASSERT_NULL(rslt);

	token_table_free(tt);
}

void tt_new_accept_zero_adds_nometadta_token(void)
{
	token_table tt = token_table_new(0);
	TEST_ASSERT_GREATER_THAN_size_t(0, capacity(tt));
	token rslt = token_table_get(tt, NMD);
	TEST_ASSERT_NOT_NULL(rslt);
	TEST_ASSERT_EQUAL_STRING("No meta data", token_text(rslt));

	token_table_free(tt);
}

void tt_grow(void)
{
	TOKEN_TABLE_KEY_T key = -1;
	token_table tt = token_table_new_empty(1);
	TEST_ASSERT_EQUAL_UINT(1, capacity(tt));

	key = token_table_add(tt, NULL);
	TEST_ASSERT_EQUAL_UINT64(0, key);
	TEST_ASSERT_EQUAL_UINT(1, capacity(tt));

	key = token_table_add(tt, NULL);
	TEST_ASSERT_EQUAL_UINT64(1, key);
	TEST_ASSERT_EQUAL_UINT(2, capacity(tt));

	key = token_table_add(tt, NULL);
	TEST_ASSERT_EQUAL_UINT64(2, key);
	TEST_ASSERT_EQUAL_UINT(4, capacity(tt));

	key = token_table_add(tt, NULL);
	TEST_ASSERT_EQUAL_UINT64(3, key);
	TEST_ASSERT_EQUAL_UINT(4, capacity(tt));

	key = token_table_add(tt, NULL);
	TEST_ASSERT_EQUAL_UINT64(4, key);
	TEST_ASSERT_EQUAL_UINT(8, capacity(tt));

	token_table_free(tt);
}

void test_token_table(void)
{
	RUN_TEST(tt_new_empty);
	RUN_TEST(tt_new_empty_accept_zero_does_not_add_nometadata_token);
	RUN_TEST(tt_new_accept_zero_adds_nometadta_token);
	RUN_TEST(tt_grow);
}
