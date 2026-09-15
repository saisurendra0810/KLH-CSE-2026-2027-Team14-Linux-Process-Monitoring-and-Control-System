#include <stdio.h>
#include <string.h>

int main() {
    /* 1. Apply Double Quotes */
    char message[] = "Hello World";
    printf("1. Double Quoted String: \"%s\"\n", message);

    /* 2. Preserve Spaces */
    char name[] = "Sai Surendranath";
    printf("2. Preserved Spaces: \"%s\"\n", name);

    /* 3. Allow Variable Expansion - simulated in C */
    int age = 20;
    printf("3. Variable Expansion: \"Age = %d\"\n", age);

    /* 4. Parse Nested Tokens */
    char input[] = "Hello World";
    char word1[20], word2[20];

    sscanf(input, "%19s %19s", word1, word2);

    printf("4. Nested Tokens: \"%s\" and \"%s\"\n", word1, word2);

    /* 5. Validate Outputs */
    if (strlen(message) > 0)
        printf("5. Output Validation: Success\n");
    else
        printf("5. Output Validation: Failed\n");

    /* 6. Test Quoted Commands */
    char command[] = "echo \"Hello World\"";
    printf("6. Quoted Command: %s\n", command);

    return 0;
}
