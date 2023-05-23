#include <errno.h>
#include <string.h>

#include "../unity/src/unity.h"
#include "../src/source.h"
#include "../src/string-builder.h"

char hello_path[] = "tests/stuff/hello-world.txt";
char hello_expected[] = "Hello, world!";

void src_file_happy(void)
{
	char c;
	source src = source_file(hello_path);
	string_builder sb = new_string_builder(0);
	while ((c = srcgetc(src))) {
		sb_addc(sb, c);
	}
	TEST_ASSERT_EQUAL_STRING(hello_expected, sb_current(sb));
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

void src_offset(void)
{
	int i = 0;
	source src = source_file(hello_path);
	for (char c; (c = srcgetc(src)); i++) {
		TEST_ASSERT_EQUAL_INT(i, source_offset(src));
	}
	TEST_ASSERT_EQUAL_INT(strlen(hello_expected), i);

	TEST_ASSERT_EQUAL_CHAR('\0', srcgetc(src));
	TEST_ASSERT_EQUAL_INT(i, source_offset(src));

	TEST_ASSERT_EQUAL_CHAR('\0', srcgetc(src));
	// TEST_ASSERT_EQUAL_INT(i, source_offset(src));
}

int test_source(void)
{
	RUN_TEST(src_file_happy);
	RUN_TEST(src_file_not_found);
	RUN_TEST(src_string_happy);
	RUN_TEST(src_string_null);
	RUN_TEST(src_offset);
}
