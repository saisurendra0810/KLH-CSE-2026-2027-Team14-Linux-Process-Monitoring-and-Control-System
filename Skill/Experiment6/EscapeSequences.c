#include <stdio.h>

int main()
{
    printf("Process Escape Sequences\n");

    printf("Hello\\World\n");
    printf("Hello\\ World\n");
    printf("Special symbols: \\$ \\& \\* \\#\n");
    printf("Escaped quotes: \\\"Hello\\\"\n");
    printf("Escaped single quote: \\'Hello\\'\n");

    printf("\nComplex Input:\n");
    printf("This is a\\ test\\ with\\ escaped\\ spaces\n");

    return 0;
}
