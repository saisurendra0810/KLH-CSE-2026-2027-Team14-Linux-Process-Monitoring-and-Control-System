#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

char variables[MAX][50];
char values[MAX][100];
int count = 0;

void setVariable(char *name, char *value)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(variables[i], name) == 0)
        {
            strcpy(values[i], value);
            return;
        }
    }

    strcpy(variables[count], name);
    strcpy(values[count], value);
    count++;
}

char *getVariable(char *name)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(variables[i], name) == 0)
            return values[i];
    }

    return NULL;
}

void expand(char *input)
{
    char result[500] = "";
    int i = 0;

    while (input[i] != '\0')
    {
        if (input[i] == '$')
        {
            char name[50];
            int j = 0;

            i++;

            if (input[i] == '{')
            {
                i++;

                while (input[i] != '}' && input[i] != '\0')
                    name[j++] = input[i++];

                name[j] = '\0';

                if (input[i] == '}')
                    i++;
            }
            else
            {
                while ((input[i] >= 'A' && input[i] <= 'Z') ||
                       (input[i] >= 'a' && input[i] <= 'z') ||
                       (input[i] >= '0' && input[i] <= '9') ||
                       input[i] == '_')
                {
                    name[j++] = input[i++];
                }

                name[j] = '\0';
            }

            char *value = getVariable(name);

            if (value != NULL)
                strcat(result, value);
            else
                printf("Undefined variable: %s\n", name);
        }
        else
        {
            int len = strlen(result);
            result[len] = input[i];
            result[len + 1] = '\0';
            i++;
        }
    }

    printf("Expanded Value: %s\n", result);
}

int main()
{
    char name[50], value[100], input[500];

    printf("Enter variable name: ");
    scanf("%49s", name);

    printf("Enter variable value: ");
    scanf("%99s", value);

    setVariable(name, value);

    printf("Enter text: ");
    getchar();
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    expand(input);

    return 0;
}
