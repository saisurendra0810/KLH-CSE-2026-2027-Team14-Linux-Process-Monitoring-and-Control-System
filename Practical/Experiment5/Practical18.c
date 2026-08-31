#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024
#define NUM_MESSAGES 10000

int main()
{
    int fd[2];
    pid_t pid;

    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    struct timeval start, end;
    double time_taken;

    // Create unnamed pipe
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Create child process
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // Parent - Producer
    if (pid > 0)
    {
        close(fd[0]);   // Close reading end

        gettimeofday(&start, NULL);

        for (int i = 0; i < NUM_MESSAGES; i++)
        {
            sprintf(message, "Message %d from Producer", i + 1);

            if (write(fd[1], message, strlen(message) + 1) == -1)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        close(fd[1]);   // Close writing end

        wait(NULL);

        gettimeofday(&end, NULL);

        time_taken =
            (end.tv_sec - start.tv_sec) +
            (end.tv_usec - start.tv_usec) / 1000000.0;

        printf("\n--- Communication Efficiency ---\n");
        printf("Number of messages : %d\n", NUM_MESSAGES);
        printf("Time taken         : %.6f seconds\n", time_taken);

        if (time_taken > 0)
        {
            printf("Messages/second    : %.2f\n",
                   NUM_MESSAGES / time_taken);
        }
    }

    // Child - Consumer
    else
    {
        close(fd[1]);   // Close writing end

        while (read(fd[0], buffer, BUFFER_SIZE) > 0)
        {
            // Consume the data
            // Uncomment the following line to display messages
            // printf("Consumer received: %s\n", buffer);
        }

        close(fd[0]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
