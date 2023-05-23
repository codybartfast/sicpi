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
	while ((c = srcgetc(src))) {
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
	while ((c = srcgetc(src))) {
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
	TEST_ASSERT_EQUAL_INT(-1, x(src));
	TEST_ASSERT_EQUAL_INT(-1, y(src));

	//o
	TEST_ASSERT_EQUAL_CHAR('o', srcgetc(src));
	TEST_ASSERT_EQUAL_INT(0, source_offset(src));
	TEST_ASSERT_EQUAL_INT(0, x(src));
	TEST_ASSERT_EQUAL_INT(0, y(src));

	//n
	srcgetc(src);

	//e
	TEST_ASSERT_EQUAL_CHAR('e', srcgetc(src));
	TEST_ASSERT_EQUAL_INT(2, source_offset(src));
	TEST_ASSERT_EQUAL_INT(2, x(src));
	TEST_ASSERT_EQUAL_INT(0, y(src));

	// \n
	srcgetc(src);

	// t
	TEST_ASSERT_EQUAL_CHAR('t', srcgetc(src));
	TEST_ASSERT_EQUAL_INT(4, source_offset(src));
	TEST_ASSERT_EQUAL_INT(0, x(src));
	TEST_ASSERT_EQUAL_INT(1, y(src));

	// w
	srcgetc(src);

	// o
	srcgetc(src);

	// \n
	TEST_ASSERT_EQUAL_CHAR('\n', srcgetc(src));
	TEST_ASSERT_EQUAL_INT(7, source_offset(src));
	TEST_ASSERT_EQUAL_INT(3, x(src));
	TEST_ASSERT_EQUAL_INT(1, y(src));

	// \n
	TEST_ASSERT_EQUAL_CHAR('\n', srcgetc(src));
	TEST_ASSERT_EQUAL_INT(8, source_offset(src));
	TEST_ASSERT_EQUAL_INT(0, x(src));
	TEST_ASSERT_EQUAL_INT(2, y(src));

	// t
	TEST_ASSERT_EQUAL_CHAR('t', srcgetc(src));
	TEST_ASSERT_EQUAL_INT(9, source_offset(src));
	TEST_ASSERT_EQUAL_INT(0, x(src));
	TEST_ASSERT_EQUAL_INT(3, y(src));

	// h
	TEST_ASSERT_EQUAL_CHAR('h', srcgetc(src));
	TEST_ASSERT_EQUAL_INT(10, source_offset(src));
	TEST_ASSERT_EQUAL_INT(1, x(src));
	TEST_ASSERT_EQUAL_INT(3, y(src));

	// r
	srcgetc(src);

	// e
	srcgetc(src);

	// e
	srcgetc(src);

	// \n
	TEST_ASSERT_EQUAL_CHAR('\n', srcgetc(src));
	TEST_ASSERT_EQUAL_INT(14, source_offset(src));
	TEST_ASSERT_EQUAL_INT(5, x(src));
	TEST_ASSERT_EQUAL_INT(3, y(src));

	// '\0'
	TEST_ASSERT_EQUAL_CHAR('\0', srcgetc(src));
	TEST_ASSERT_EQUAL_INT(14, source_offset(src));
	TEST_ASSERT_EQUAL_INT(5, x(src));
	TEST_ASSERT_EQUAL_INT(3, y(src));

	// '\0'
	TEST_ASSERT_EQUAL_CHAR('\0', srcgetc(src));
	TEST_ASSERT_EQUAL_INT(14, source_offset(src));
	TEST_ASSERT_EQUAL_INT(5, x(src));
	TEST_ASSERT_EQUAL_INT(3, y(src));
}

int test_source(void)
{
	RUN_TEST(src_file_happy);
	RUN_TEST(src_file_not_found);
	RUN_TEST(src_string_happy);
	RUN_TEST(src_string_null);
	RUN_TEST(src_position);
}
