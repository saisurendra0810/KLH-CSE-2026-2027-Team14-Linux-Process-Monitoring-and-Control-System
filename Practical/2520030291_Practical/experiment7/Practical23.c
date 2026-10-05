#include <stdio.h>
#include <stdlib.h>

int global_var = 10;

void code_function()
{
    printf("Address of Code:   %p\n", (void *)code_function);
}

int main()
{
    static int static_var = 20;
    int stack_var = 30;
    int *heap_var = (int *)malloc(sizeof(int));

    if (heap_var == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    *heap_var = 40;

    code_function();

    printf("Address of Global:  %p\n", (void *)&global_var);
    printf("Address of Static:  %p\n", (void *)&static_var);
    printf("Address of Heap:    %p\n", (void *)heap_var);
    printf("Address of Stack:   %p\n", (void *)&stack_var);

    free(heap_var);

    return 0;
}
