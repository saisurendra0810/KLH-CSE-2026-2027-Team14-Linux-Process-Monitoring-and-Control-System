#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 100
#define MAX_ARGS 10

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    pid_t pid;

    while (1) {
        printf("my_shell> ");
        fgets(input, MAX_INPUT, stdin);

        // Remove newline character
        input[strcspn(input, "\n")] = '\0';

        // Exit command
        if (strcmp(input, "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }

        // Split input into arguments
        int i = 0;
        args[i] = strtok(input, " ");

        while (args[i] != NULL && i < MAX_ARGS - 1) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        // Create child process
        pid = fork();

        if (pid < 0) {
            perror("Fork Failed");
            exit(1);
        }
        else if (pid == 0) {
            // Child Process
            printf("Child Process (PID = %d)\n", getpid());

            // Execute command
            execvp(args[0], args);

            // If exec fails
            perror("Command Execution Failed");
            exit(1);
        }
        else {
            // Parent Process
            printf("Parent Process (PID = %d)\n", getpid());

            // Wait for child to finish
            wait(NULL);

            printf("Child process completed.\n\n");
        }
    }

    return 0;
}
