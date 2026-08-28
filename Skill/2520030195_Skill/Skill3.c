#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 16

typedef struct HistoryNode
{
    char *command;
    struct HistoryNode *prev;
    struct HistoryNode *next;
} HistoryNode;

typedef struct
{
    char *buffer;
    size_t size;
    size_t length;
} InputBuffer;

HistoryNode *head = NULL;
HistoryNode *tail = NULL;
HistoryNode *current = NULL;

/* Initialize input buffer */
void init_buffer(InputBuffer *input)
{
    input->size = INITIAL_BUFFER_SIZE;
    input->length = 0;

    input->buffer = malloc(input->size);

    if (input->buffer == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    input->buffer[0] = '\0';
}

/* Resize input buffer */
void resize_buffer(InputBuffer *input)
{
    input->size *= 2;

    char *temp = realloc(input->buffer, input->size);

    if (temp == NULL)
    {
        free(input->buffer);
        perror("realloc");
        exit(EXIT_FAILURE);
    }

    input->buffer = temp;
}

/* Read input dynamically */
void read_input(InputBuffer *input)
{
    int ch;

    input->length = 0;
    input->buffer[0] = '\0';

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        if (input->length + 1 >= input->size)
        {
            resize_buffer(input);
        }

        input->buffer[input->length++] = (char)ch;
        input->buffer[input->length] = '\0';
    }
}

/* Add command to history */
void add_history(const char *command)
{
    HistoryNode *new_node;

    new_node = malloc(sizeof(HistoryNode));

    if (new_node == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    new_node->command = malloc(strlen(command) + 1);

    if (new_node->command == NULL)
    {
        free(new_node);
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strcpy(new_node->command, command);

    new_node->prev = tail;
    new_node->next = NULL;

    if (tail != NULL)
    {
        tail->next = new_node;
    }
    else
    {
        head = new_node;
    }

    tail = new_node;

    current = NULL;
}

/* Display command history */
void show_history()
{
    HistoryNode *temp = head;
    int number = 1;

    printf("\n\033[1;34mCommand History:\033[0m\n");

    if (temp == NULL)
    {
        printf("No commands in history.\n");
        return;
    }

    while (temp != NULL)
    {
        printf("%d  %s\n", number, temp->command);

        temp = temp->next;
        number++;
    }
}

/* Navigate to previous command */
char *previous_command()
{
    if (tail == NULL)
    {
        printf("No command history.\n");
        return NULL;
    }

    if (current == NULL)
    {
        current = tail;
    }
    else if (current->prev != NULL)
    {
        current = current->prev;
    }
    else
    {
        printf("Already at oldest command.\n");
    }

    return current->command;
}

/* Navigate to next command */
char *next_command()
{
    if (current == NULL)
    {
        printf("Already at newest command.\n");
        return NULL;
    }

    if (current->next != NULL)
    {
        current = current->next;
        return current->command;
    }

    current = NULL;

    printf("No newer command.\n");

    return NULL;
}

/* Update input buffer */
void update_buffer(InputBuffer *input, const char *command)
{
    size_t required = strlen(command) + 1;

    while (input->size < required)
    {
        resize_buffer(input);
    }

    strcpy(input->buffer, command);

    input->length = strlen(command);
}

/* Free command history */
void free_history()
{
    HistoryNode *temp;

    while (head != NULL)
    {
        temp = head;

        head = head->next;

        free(temp->command);
        free(temp);
    }

    tail = NULL;
    current = NULL;
}

/* Display memory information */
void display_buffer_info(InputBuffer *input)
{
    printf("\n\033[1;33mInput Buffer Information:\033[0m\n");
    printf("Buffer size   : %zu bytes\n", input->size);
    printf("Current length: %zu bytes\n", input->length);
}

/* Main function */
int main()
{
    InputBuffer input;

    init_buffer(&input);

    printf("\033[1;32m========================================\033[0m\n");
    printf("\033[1;32m Command History and Dynamic Buffer Demo \033[0m\n");
    printf("\033[1;32m========================================\033[0m\n");

    printf("\nCommands:\n");
    printf("  history   - Show command history\n");
    printf("  prev      - Previous command\n");
    printf("  next      - Next command\n");
    printf("  buffer    - Show buffer information\n");
    printf("  exit      - Exit program\n");

    while (1)
    {
        printf("\n\033[1;36mshell> \033[0m");

        read_input(&input);

        if (strcmp(input.buffer, "exit") == 0)
        {
            break;
        }

        if (strcmp(input.buffer, "history") == 0)
        {
            show_history();
            continue;
        }

        if (strcmp(input.buffer, "prev") == 0)
        {
            char *command = previous_command();

            if (command != NULL)
            {
                update_buffer(&input, command);

                printf("\033[1;35mRecalled: %s\033[0m\n",
                       input.buffer);
            }

            continue;
        }

        if (strcmp(input.buffer, "next") == 0)
        {
            char *command = next_command();

            if (command != NULL)
            {
                update_buffer(&input, command);

                printf("\033[1;35mRecalled: %s\033[0m\n",
                       input.buffer);
            }

            continue;
        }

        if (strcmp(input.buffer, "buffer") == 0)
        {
            display_buffer_info(&input);
            continue;
        }

        if (strlen(input.buffer) > 0)
        {
            add_history(input.buffer);

            printf("\033[1;32mCommand stored:\033[0m %s\n",
                   input.buffer);
        }
    }

    printf("\n\033[1;33mReleasing memory...\033[0m\n");

    free(input.buffer);

    free_history();

    printf("\033[1;32mAll dynamically allocated memory released.\033[0m\n");

    return 0;
}
