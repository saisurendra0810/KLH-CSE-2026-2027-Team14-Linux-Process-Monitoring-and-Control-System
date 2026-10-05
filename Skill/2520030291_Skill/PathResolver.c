#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    char *path;
    char *path_copy;
    char *directory;
    char command[100];
    char full_path[500];
    int found = 0;

    path = getenv("PATH");

    if (path == NULL)
    {
        printf("PATH variable not found.\n");
        return 1;
    }

    path_copy = strdup(path);

    if (path_copy == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter command to search: ");
    scanf("%99s", command);

    directory = strtok(path_copy, ":");

    while (directory != NULL)
    {
        snprintf(full_path, sizeof(full_path),
                 "%s/%s", directory, command);

        if (access(full_path, X_OK) == 0)
        {
            printf("Executable found: %s\n", full_path);
            found = 1;
            break;
        }

        directory = strtok(NULL, ":");
    }

    if (!found)
    {
        printf("Command not found: %s\n", command);
    }

    free(path_copy);

    return 0;
}
