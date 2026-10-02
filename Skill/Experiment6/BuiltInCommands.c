#include <stdio.h>
#include <string.h>
#include <unistd.h>

void pwd()
{
    char path[500];

    if (getcwd(path, sizeof(path)) != NULL)
        printf("Current Directory: %s\n", path);
}

void show()
{
    printf("Built-in command executed successfully.\n");
}

void help()
{
    printf("Available commands:\n");
    printf("pwd\n");
    printf("show\n");
    printf("help\n");
    printf("exit\n");
}

typedef void (*CommandFunction)();

struct Command
{
    char name[20];
    CommandFunction function;
};

struct Command commands[] =
{
    {"pwd", pwd},
    {"show", show},
    {"help", help}
};

int commandCount = 3;

void executeCommand(char *command)
{
    for (int i = 0; i < commandCount; i++)
    {
        if (strcmp(command, commands[i].name) == 0)
        {
            commands[i].function();
            return;
        }
    }

    printf("Invalid command: %s\n", command);
}

int main()
{
    char command[50];

    while (1)
    {
        printf("\nEnter command: ");
        scanf("%49s", command);

        if (strcmp(command, "exit") == 0)
            break;

        executeCommand(command);
    }

    printf("Program terminated.\n");

    return 0;
}
