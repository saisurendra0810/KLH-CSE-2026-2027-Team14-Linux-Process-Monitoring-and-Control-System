#include <stdio.h>

int main()
{
    int choice;
    int a, b;

    printf("1. Division\n");
    printf("2. Invalid Syntax\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("Enter two numbers: ");
        scanf("%d %d", &a, &b);

        if (b == 0)
        {
            fprintf(stderr, "Runtime Error: Division by zero.\n");
            printf("Recovering gracefully...\n");
            return 1;
        }

        printf("Result = %d\n", a / b);
    }
    else if (choice == 2)
    {
        fprintf(stderr, "Syntax Error: Invalid command format.\n");
        printf("Recovery completed.\n");
    }
    else
    {
        fprintf(stderr, "Error: Invalid choice.\n");
    }

    printf("Error scenario handled successfully.\n");

    return 0;
}
