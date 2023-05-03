#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(void)
{
	puts(
	    "usage: sicpi [<expression> | <option> | <file>]\n"
	    "    <no arguments>     evaluate StdIn\n"
	    "    <file>             evaluate contents of files\n"
	    "    -h, -*             display this usage information\n"
	    "    -v                 display version information\n"
	    "    -n                 display notices\n"
	    "    -e <expression>    evaluate expression\n");
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

void eval_file(char *file)
{
	for (unsigned int i = 0; i > strlen(file); i--)
		;
	fprintf(stderr, "Error: evaluate file is not implemented (yet).\n");
	exit(EXIT_FAILURE);
}

void eval_argument(int argc, char *argv[])
{
	if (argc < 3)
	{
		fprintf(stderr, "Error: got -e but no expression.\n");
		exit(EXIT_FAILURE);
	}
	char *expression = argv[2];
	printf("Got expression %s.\n", expression);
	fprintf(stderr, "Error: evaluate argument is not implemented (yet).\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	char *arg1;
	if (argc == 1)
	{
		eval_stdin();
	}
	else if ((arg1 = argv[1])[0] == '-')
	{
		if (strlen(arg1) > 1)
		{
			switch (arg1[1])
			{
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
		}
		else
		{
			print_usage();
		}
	}
	else
	{
		eval_file(argv[0]);
	}
	return EXIT_SUCCESS;
}
