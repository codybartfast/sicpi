#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(void)
{
	puts(
	    "usage: sicpi [<expression> | <option> | <file>]\n"
	    "    <expression> begining with '('     evaluate expression\n"
	    "    -h, -*                             display this usage information\n"
	    "    -v                                 display version information\n"
	    "    -n                                 display notices\n"
	    "    <file>                             evaluate contents of files\n"
	    "    <no arguments>                     evaluate StdIn\n"
	    "\n");
}

void print_version(void)
{
	fprintf(stderr, "Error: display version is not implemented (yet).\n");
	exit(EXIT_FAILURE);
}

void print_notices(void)
{
	fprintf(stderr, "Error: display notices is not implemented (yet).\n");
	exit(EXIT_FAILURE);
}

void eval_stdin(void)
{
	fprintf(stderr, "Error: evaluate stdin is not implemented (yet).\n");
	exit(EXIT_FAILURE);
}

void eval_argument(char *expr)
{
	fprintf(stderr, "Error: evaluate argument is not implemented (yet).\n");
	exit(EXIT_FAILURE);
}

void eval_file(char *file)
{
	fprintf(stderr, "Error: evaluate file is not implemented (yet).\n");
	exit(EXIT_FAILURE);
}

void option(char *opt)
{
	if (strlen(opt) > 1)
	{
		switch (opt[1])
		{
		case 'V':
		case 'v':
			print_version();
			break;
		case 'N':
		case 'n':
			print_notices();
			break;
		default:
			print_usage();
		}
	}
	else
	{
		print_usage();
	}
}

int main(int argc, char *argv[])
{
	char *arg1;
	switch (argc)
	{
	case 1:
		eval_stdin();
		break;
	case 2:
		arg1 = argv[1];
		switch (arg1[0])
		{
		case '(':
			eval_argument(arg1);
			break;
		case '-':
			option(arg1);
			break;
		default:
			eval_file(arg1);
			break;
		}
		break; // should be unreachable
	default:
		fprintf(stderr, "Error: expected 0 or 1 arguments, got %d\n", argc - 1);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
