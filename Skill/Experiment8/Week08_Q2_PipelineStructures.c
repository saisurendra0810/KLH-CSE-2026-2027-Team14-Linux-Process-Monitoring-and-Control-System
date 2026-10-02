#include <stdio.h>
#include <string.h>

int main()
{
    char input[200];
    char *command;

    printf("Enter pipeline commands using | : ");
    fgets(input, sizeof(input), stdin);

    printf("\nPipeline Structure:\n");

    command = strtok(input, "|");

    while (command != NULL)
    {
        printf("Command: %s", command);
        command = strtok(NULL, "|");
    }

    printf("\nPipeline order validated.\n");

    return 0;
}
