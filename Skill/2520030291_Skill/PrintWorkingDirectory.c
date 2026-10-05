#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

int main()
{
    char path[PATH_MAX];

    if (getcwd(path, sizeof(path)) == NULL)
    {
        perror("getcwd");
        return 1;
    }

    printf("Current Directory: %s\n", path);

    printf("Press Enter to exit...");
    getchar();
    getchar();

    printf("Cleaning up resources...\n");

    return 0;
}
