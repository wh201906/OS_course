#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("\033[1;33m"); // yellow, bold
    printf("Test\n");
    getc(stdin);
    return 0;
}