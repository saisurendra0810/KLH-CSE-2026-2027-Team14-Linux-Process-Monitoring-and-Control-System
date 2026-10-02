#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *data;

    data = malloc(5 * sizeof(int));

    if (data == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < 5; i++)
        data[i] = i + 1;

    printf("Allocated values: ");

    for (int i = 0; i < 5; i++)
        printf("%d ", data[i]);

    printf("\n");

    free(data);
    data = NULL;

    printf("Memory released successfully.\n");
    printf("Use Valgrind with:\n");
    printf("valgrind --leak-check=full ./Week13_Q3_ValgrindMemoryCheck\n");

    return 0;
}
