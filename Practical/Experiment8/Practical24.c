#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int i;

    printf("========================================\n");
    printf("        WEEK 8 - OSSP PRACTICAL\n");
    printf("========================================\n");

    // ----------------------------------------
    // PART 1: malloc()
    // ----------------------------------------

    printf("\n--- malloc() ---\n");

    int *ptr = (int *)malloc(3 * sizeof(int));

    if (ptr == NULL)
    {
        printf("malloc failed.\n");
        return 1;
    }

    for (i = 0; i < 3; i++)
    {
        ptr[i] = (i + 1) * 10;
    }

    printf("Values using malloc: ");

    for (i = 0; i < 3; i++)
    {
        printf("%d ", ptr[i]);
    }

    printf("\n");


    // ----------------------------------------
    // PART 2: calloc()
    // ----------------------------------------

    printf("\n--- calloc() ---\n");

    int *cptr = (int *)calloc(3, sizeof(int));

    if (cptr == NULL)
    {
        printf("calloc failed.\n");
        free(ptr);
        return 1;
    }

    printf("Values using calloc: ");

    for (i = 0; i < 3; i++)
    {
        printf("%d ", cptr[i]);
    }

    printf("\n");


    // ----------------------------------------
    // PART 3: realloc()
    // ----------------------------------------

    printf("\n--- realloc() ---\n");

    ptr = (int *)realloc(ptr, 5 * sizeof(int));

    if (ptr == NULL)
    {
        printf("realloc failed.\n");
        free(cptr);
        return 1;
    }

    ptr[3] = 40;
    ptr[4] = 50;

    printf("Values after realloc: ");

    for (i = 0; i < 5; i++)
    {
        printf("%d ", ptr[i]);
    }

    printf("\n");


    // ----------------------------------------
    // PART 4: free()
    // ----------------------------------------

    printf("\n--- free() ---\n");

    free(ptr);
    free(cptr);

    printf("Allocated memory successfully freed.\n");


    // ----------------------------------------
    // PART 5: Copy-on-Write using fork()
    // ----------------------------------------

    printf("\n========================================\n");
    printf("       COPY-ON-WRITE USING fork()\n");
    printf("========================================\n");

    int data = 100;

    printf("\nBefore fork():\n");
    printf("Parent PID  : %d\n", getpid());
    printf("Data value  : %d\n", data);
    printf("Data address: %p\n", (void *)&data);

    pid_t pid = fork();

    if (pid < 0)
    {
        printf("Fork failed.\n");
        return 1;
    }

    if (pid == 0)
    {
        // Child process

        printf("\n--- Child Process ---\n");

        printf("Child PID   : %d\n", getpid());
        printf("Data before modification: %d\n", data);
        printf("Address before modification: %p\n",
               (void *)&data);

        // Modify data
        data = 200;

        printf("\nAfter modifying data in child:\n");
        printf("Child data value: %d\n", data);
        printf("Child data address: %p\n",
               (void *)&data);
    }
    else
    {
        // Parent process

        wait(NULL);

        printf("\n--- Parent Process ---\n");

        printf("Parent PID  : %d\n", getpid());
        printf("Parent data value: %d\n", data);
        printf("Parent data address: %p\n",
               (void *)&data);

        printf("\nCopy-on-Write demonstration completed.\n");
    }

    return 0;
}
