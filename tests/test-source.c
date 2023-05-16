#include <errno.h>

#include "../unity/src/unity.h"
#include "../src/source.h"
#include "../src/string-builder.h"

void src_file_happy(void)
{
	char c;
	source src = source_file("tests/stuff/hello-world.txt");
	string_builder sb = new_string_builder(0);
	while ((c = srcgetc(src))) {
		sb_addc(sb, c);
	}
	char expected[] = "Hello, world!";
	TEST_ASSERT_EQUAL_STRING(expected, sb_current(sb));
	sb_free(sb);
	source_close(src);
}

void src_file_not_found(void)
{
	TEST_ASSERT_NULL(source_file("not valid file path"));
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
	TEST_ASSERT_NULL(source_string(NULL));
}

int test_source(void)
{
	RUN_TEST(src_file_happy);
	RUN_TEST(src_file_not_found);
	RUN_TEST(src_string_happy);
	RUN_TEST(src_string_null);
}
