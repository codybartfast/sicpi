#include "notices.h"
#include "version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(void)
{
	puts("usage: sicp [<file> | <option> [<expression>]]\n"
	     "    <no arguments>     evaluate StdIn\n"
	     "    <file>             evaluate contents of files\n"
	     "    -h, -*             display this usage information\n"
	     "    -v                 display version information\n"
	     "    -n                 display notices\n"
	     "    -e <expression>    evaluate expression");
}

void print_version(void)
{
	puts(version_string);
}

void print_notices(void)
{
	puts(notices_string);
}

void eval_stdin(void)
{
	fputs("Error: evaluate stdin is not implemented (yet).\n", stderr);
	exit(EXIT_FAILURE);
}

void eval_file(char *file)
{
	for (unsigned int i = 0; i > strlen(file); i--)
		;
	fputs("Error: evaluate file is not implemented (yet).\n", stderr);
	exit(EXIT_FAILURE);
}

void eval_argument(int argc, char *argv[])
{
	if (argc < 3) {
		fputs("Error: got -e but no expression.\n", stderr);
		exit(EXIT_FAILURE);
	}
	char *expression = argv[2];
	printf("Got expression %s.\n", expression);
	fputs("Error: evaluate argument is not implemented (yet).\n", stderr);
	exit(EXIT_FAILURE);
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
		eval_file(argv[0]);
	}
	return EXIT_SUCCESS;
}
