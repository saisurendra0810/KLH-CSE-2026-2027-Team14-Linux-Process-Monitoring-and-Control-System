#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int validName(char *name)
{
    if (!isalpha(name[0]) && name[0] != '_')
        return 0;

    for (int i = 1; name[i] != '\0'; i++)
    {
        if (!isalnum(name[i]) && name[i] != '_')
            return 0;
    }

    return 1;
}

int main()
{
    char input[200];
    char name[100];
    char value[100];

    printf("Enter export command: ");
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    if (strncmp(input, "export ", 7) != 0)
    {
        printf("Invalid export syntax.\n");
        return 1;
    }

    char *assignment = input + 7;
    char *equal = strchr(assignment, '=');

    if (equal == NULL)
    {
        printf("Invalid export syntax.\n");
        return 1;
    }

    int nameLength = equal - assignment;

    strncpy(name, assignment, nameLength);
    name[nameLength] = '\0';

    strcpy(value, equal + 1);

    if (!validName(name))
    {
        printf("Invalid variable name.\n");
        return 1;
    }

    if (setenv(name, value, 1) != 0)
    {
        perror("setenv");
        return 1;
    }

    printf("Environment variable updated successfully.\n");

    printf("%s = %s\n", name, getenv(name));

    return 0;
}
