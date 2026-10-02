#include <stdio.h>

int add(int a, int b)
{
    return a + b;
}

int main()
{
    int passed = 0;
    int total = 4;

    printf("Test 1: ");
    if (add(2, 3) == 5)
    {
        printf("PASS\n");
        passed++;
    }
    else
        printf("FAIL\n");

    printf("Test 2: ");
    if (add(10, 5) == 15)
    {
        printf("PASS\n");
        passed++;
    }
    else
        printf("FAIL\n");

    printf("Test 3: ");
    if (add(0, 0) == 0)
    {
        printf("PASS\n");
        passed++;
    }
    else
        printf("FAIL\n");

    printf("Test 4: ");
    if (add(-2, 2) == 0)
    {
        printf("PASS\n");
        passed++;
    }
    else
        printf("FAIL\n");

    printf("\nTest Report\n");
    printf("Passed: %d/%d\n", passed, total);
    printf("Failed: %d/%d\n", total - passed, total);

    return 0;
}
