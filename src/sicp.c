#include "object.h"
#include "notices.h"
#include "run.h"
#include "source.h"
#include "version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_usage(void)
{
	puts("usage: sicp [<file> | <option> [<expression>]]\n"
	     "    <no arguments>     evaluate StdIn\n"
	     "    <file>             evaluate contents of files\n"
	     "    -h, -*             display this usage information\n"
	     "    -v                 display version information\n"
	     "    -n                 display notices\n"
	     "    -e <expression>    evaluate expression");
}

static void print_version(void)
{
	puts(version_string);
}

static void print_notices(void)
{
	puts(notices_string);
}

static void run_fun_test(void)
{
	puts("Watch this space.");
}

static void execute(source src)
{
	object rslt = load_run(src);
	printf("result: %s\n", to_text(rslt));
	exit(EXIT_SUCCESS);
}

static void eval_stdin(void)
{
	source src = source_stream(stdin, "STDIN");
	execute(src);
}

static void eval_file(char *file)
{
	source src = source_file(file);
	if (!src) {
		exit(EXIT_FAILURE);
	}
	execute(src);
}

static void eval_argument(int argc, char *argv[])
{
	if (argc < 3) {
		fputs("Error: got -e but no expression.\n", stderr);
		exit(EXIT_FAILURE);
	}
	char *expression = argv[2];
	source src = source_string(expression, "command-line-argument");
	execute(src);
}

int main(int argc, char *argv[])
{
	char *arg1;
	if (argc == 1) {
		eval_stdin();
	} else if ((arg1 = argv[1])[0] == '-') {
		if (strlen(arg1) > 1) {
			switch (arg1[1]) {
			case 'e':
				eval_argument(argc, argv);
				break;
			case 'n':
				print_notices();
				break;
			case 't':
				run_fun_test();
				break;
			case 'v':
				print_version();
				break;
			default:
				print_usage();
				break;
			}
		} else {
			print_usage();
		}
	} else {
		eval_file(argv[1]);
	}
	return EXIT_SUCCESS;
}
