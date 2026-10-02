#include <stdio.h>
#include <string.h>

#define MAX_HISTORY 10
#define MAX_CMD 100

int main()
{
    char history[MAX_HISTORY][MAX_CMD];
    int count = 0;
    int choice;
    char command[MAX_CMD];

    while (1)
    {
        printf("\n1. Add Command\n2. Display History\n3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1)
        {
            printf("Enter command: ");
            fgets(command, sizeof(command), stdin);
            command[strcspn(command, "\n")] = '\0';

            if (count < MAX_HISTORY)
            {
                strcpy(history[count], command);
                count++;
            }
            else
            {
                for (int i = 1; i < MAX_HISTORY; i++)
                    strcpy(history[i - 1], history[i]);

                strcpy(history[MAX_HISTORY - 1], command);
            }

            printf("Command stored successfully.\n");
        }
        else if (choice == 2)
        {
            printf("\nCommand History:\n");
            for (int i = 0; i < count; i++)
                printf("%d  %s\n", i + 1, history[i]);
        }
        else if (choice == 3)
        {
            break;
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
