#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

int main()
{
    char path[PATH_MAX];
    char current[PATH_MAX];
    char previous[PATH_MAX];

    if (getcwd(current, sizeof(current)) == NULL)
    {
        perror("getcwd");
        return 1;
    }

    printf("Current Directory: %s\n", current);

    printf("Enter directory to change: ");
    scanf("%s", path);

    strcpy(previous, current);

    if (chdir(path) == 0)
    {
        if (getcwd(current, sizeof(current)) != NULL)
        {
            printf("Previous Directory: %s\n", previous);
            printf("New Directory: %s\n", current);
        }
    }
    else
    {
        perror("chdir");
    }

    return 0;
}
