#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    if (puts("Hello, wolrd!") == EOF)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}
