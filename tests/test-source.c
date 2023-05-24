#include <errno.h>
#include <string.h>

#include "../unity/src/unity.h"
#include "../src/source.h"
#include "../src/string-builder.h"

char numbers_path[] = "tests/stuff/numbers.txt";
char numbers_expected[] = "one\ntwo\n\nthree\n";

void src_file_happy(void)
{
	char c;
	source src = source_file(numbers_path);
	string_builder sb = new_string_builder(0);
	while ((c = source_c(src))) {
		sb_addc(sb, c);
	}
	TEST_ASSERT_EQUAL_STRING(numbers_expected, sb_current(sb));
	sb_free(sb);
	source_close(src);
}

void src_file_not_found(void)
{
	source src = source_file("not valid file path");
	TEST_ASSERT_NULL(src);
	source_close(src);
}

void src_string_happy(void)
{
	char *text = "Lorem\nIpsum";
	char c;
	source src = source_string(text);
	string_builder sb = new_string_builder(0);
	while ((c = source_c(src))) {
		sb_addc(sb, c);
	}
	char expected[] = "Lorem\nIpsum";
	TEST_ASSERT_EQUAL_STRING(expected, sb_current(sb));
	sb_free(sb);
	source_close(src);
}

void src_string_null(void)
{
	source src = source_string(NULL);
	TEST_ASSERT_NULL(src);
	source_close(src);
}

void src_position(void)
{
	int i = -1;
	source src = source_file(numbers_path);

	TEST_ASSERT_EQUAL_INT(-1, source_offset(src));
	TEST_ASSERT_EQUAL_INT(-1, source_x(src));
	TEST_ASSERT_EQUAL_INT(-1, source_y(src));

	//o
	TEST_ASSERT_EQUAL_CHAR('o', source_c(src));
	TEST_ASSERT_EQUAL_INT(0, source_offset(src));
	TEST_ASSERT_EQUAL_INT(0, source_x(src));
	TEST_ASSERT_EQUAL_INT(0, source_y(src));

	//n
	source_c(src);

	//e
	TEST_ASSERT_EQUAL_CHAR('e', source_c(src));
	TEST_ASSERT_EQUAL_INT(2, source_offset(src));
	TEST_ASSERT_EQUAL_INT(2, source_x(src));
	TEST_ASSERT_EQUAL_INT(0, source_y(src));

	// \n
	source_c(src);

	// t
	TEST_ASSERT_EQUAL_CHAR('t', source_c(src));
	TEST_ASSERT_EQUAL_INT(4, source_offset(src));
	TEST_ASSERT_EQUAL_INT(0, source_x(src));
	TEST_ASSERT_EQUAL_INT(1, source_y(src));

	// w
	source_c(src);

	// o
	source_c(src);

	// \n
	TEST_ASSERT_EQUAL_CHAR('\n', source_c(src));
	TEST_ASSERT_EQUAL_INT(7, source_offset(src));
	TEST_ASSERT_EQUAL_INT(3, source_x(src));
	TEST_ASSERT_EQUAL_INT(1, source_y(src));

	// \n
	TEST_ASSERT_EQUAL_CHAR('\n', source_c(src));
	TEST_ASSERT_EQUAL_INT(8, source_offset(src));
	TEST_ASSERT_EQUAL_INT(0, source_x(src));
	TEST_ASSERT_EQUAL_INT(2, source_y(src));

	// t
	TEST_ASSERT_EQUAL_CHAR('t', source_c(src));
	TEST_ASSERT_EQUAL_INT(9, source_offset(src));
	TEST_ASSERT_EQUAL_INT(0, source_x(src));
	TEST_ASSERT_EQUAL_INT(3, source_y(src));

	// h
	TEST_ASSERT_EQUAL_CHAR('h', source_c(src));
	TEST_ASSERT_EQUAL_INT(10, source_offset(src));
	TEST_ASSERT_EQUAL_INT(1, source_x(src));
	TEST_ASSERT_EQUAL_INT(3, source_y(src));

	// r
	source_c(src);

	// e
	source_c(src);

	// e
	source_c(src);

	// \n
	TEST_ASSERT_EQUAL_CHAR('\n', source_c(src));
	TEST_ASSERT_EQUAL_INT(14, source_offset(src));
	TEST_ASSERT_EQUAL_INT(5, source_x(src));
	TEST_ASSERT_EQUAL_INT(3, source_y(src));

	// '\0'
	TEST_ASSERT_EQUAL_CHAR('\0', source_c(src));
	TEST_ASSERT_EQUAL_INT(14, source_offset(src));
	TEST_ASSERT_EQUAL_INT(5, source_x(src));
	TEST_ASSERT_EQUAL_INT(3, source_y(src));

	// '\0'
	TEST_ASSERT_EQUAL_CHAR('\0', source_c(src));
	TEST_ASSERT_EQUAL_INT(14, source_offset(src));
	TEST_ASSERT_EQUAL_INT(5, source_x(src));
	TEST_ASSERT_EQUAL_INT(3, source_y(src));
}

void src_peek(void)
{
	source src = source_file(numbers_path);

	// o
	TEST_ASSERT_EQUAL_INT(-1, source_offset(src));
	TEST_ASSERT_EQUAL_INT(-1, source_x(src));
	TEST_ASSERT_EQUAL_INT(-1, source_y(src));

	TEST_ASSERT_EQUAL_CHAR('o', source_peek(src));
	TEST_ASSERT_EQUAL_CHAR('o', source_peek(src));

	TEST_ASSERT_EQUAL_INT(-1, source_offset(src));
	TEST_ASSERT_EQUAL_INT(-1, source_x(src));
	TEST_ASSERT_EQUAL_INT(-1, source_y(src));

	TEST_ASSERT_EQUAL_CHAR('o', source_c(src));

	TEST_ASSERT_EQUAL_INT(0, source_offset(src));
	TEST_ASSERT_EQUAL_INT(0, source_x(src));
	TEST_ASSERT_EQUAL_INT(0, source_y(src));

	//n
	source_c(src);

	//e
	source_c(src);

	// \n
	source_c(src);

	// t
	TEST_ASSERT_EQUAL_CHAR('t', source_peek(src));
	TEST_ASSERT_EQUAL_INT(3, source_offset(src));
	TEST_ASSERT_EQUAL_INT(3, source_x(src));
	TEST_ASSERT_EQUAL_INT(0, source_y(src));

	TEST_ASSERT_EQUAL_CHAR('t', source_c(src));
	TEST_ASSERT_EQUAL_INT(4, source_offset(src));
	TEST_ASSERT_EQUAL_INT(0, source_x(src));
	TEST_ASSERT_EQUAL_INT(1, source_y(src));

	// wo\n\nthree

	for (int i = 0; i < 9; i++)
		source_c(src);

	// \n
	TEST_ASSERT_EQUAL_CHAR('\n', source_c(src));
	TEST_ASSERT_EQUAL_INT(14, source_offset(src));
	TEST_ASSERT_EQUAL_INT(5, source_x(src));
	TEST_ASSERT_EQUAL_INT(3, source_y(src));

	// '\0' peek ok
	TEST_ASSERT_EQUAL_CHAR('\0', source_peek(src));
	TEST_ASSERT_EQUAL_INT(14, source_offset(src));
	TEST_ASSERT_EQUAL_INT(5, source_x(src));
	TEST_ASSERT_EQUAL_INT(3, source_y(src));

	// can repeat
	TEST_ASSERT_EQUAL_CHAR('\0', source_peek(src));
	TEST_ASSERT_EQUAL_INT(14, source_offset(src));
	TEST_ASSERT_EQUAL_INT(5, source_x(src));
	TEST_ASSERT_EQUAL_INT(3, source_y(src));

	// try read more
	TEST_ASSERT_EQUAL_CHAR('\0', source_c(src));
	TEST_ASSERT_EQUAL_INT(14, source_offset(src));
	TEST_ASSERT_EQUAL_INT(5, source_x(src));
	TEST_ASSERT_EQUAL_INT(3, source_y(src));

	// peek returns something sensible.
	TEST_ASSERT_EQUAL_CHAR('\0', source_peek(src));
	TEST_ASSERT_EQUAL_INT(14, source_offset(src));
	TEST_ASSERT_EQUAL_INT(5, source_x(src));
	TEST_ASSERT_EQUAL_INT(3, source_y(src));
}

int test_source(void)
{
	RUN_TEST(src_file_happy);
	RUN_TEST(src_file_not_found);
	RUN_TEST(src_string_happy);
	RUN_TEST(src_string_null);
	RUN_TEST(src_position);
	RUN_TEST(src_peek);
}
