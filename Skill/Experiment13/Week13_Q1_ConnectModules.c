#include <stdio.h>

int add(int a, int b)
{
    return a + b;
}

int multiply(int a, int b)
{
    return a * b;
}

int main()
{
    int a, b;

    printf("Enter two numbers: ");
    scanf("%d %d", &a, &b);

    printf("Module 1 - Addition = %d\n", add(a, b));
    printf("Module 2 - Multiplication = %d\n", multiply(a, b));

    printf("Interfaces connected successfully.\n");
    printf("End-to-end execution completed.\n");

    return 0;
}
