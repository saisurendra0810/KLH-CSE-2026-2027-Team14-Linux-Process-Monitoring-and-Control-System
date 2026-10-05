#include <stdio.h>
#include <string.h>

int main() {
    /* 1. Apply Single Quotes */
    char ch = 'A';
    printf("1. Single Quote Character: %c\n", ch);

    /* 2. Preserve Literal Content */
    char literal[] = "$name";
    printf("2. Literal Content: %s\n", literal);

    /* 3. Ignore Variable Expansion */
    char name[] = "Sai";
    char text[] = "$name";
    printf("3. Variable Expansion Ignored: %s\n", text);
    printf("   Actual Variable: %s\n", name);

    /* 4. Store Quoted Strings */
    char message[] = "Hello World";
    printf("4. Stored Quoted String: %s\n", message);

    /* 5. Validate Parsing Results */
    char word[20];
    int result = sscanf("Hello", "%19s", word);

    if (result == 1)
        printf("5. Parsing Result: Success - %s\n", word);
    else
        printf("5. Parsing Result: Failed\n");

    /* 6. Test Edge Cases */
    char empty[] = "";
    char special[] = "$ @ # !";

    printf("6. Edge Case - Empty String: '%s'\n", empty);
    printf("   Edge Case - Special Characters: %s\n", special);

    return 0;
}
