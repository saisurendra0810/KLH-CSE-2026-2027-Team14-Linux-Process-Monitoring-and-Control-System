#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("\nReceived SIGINT (Ctrl+C)\n");
        printf("Interrupt signal handled.\n");
        printf("Program terminated.\n");
        exit(0);
    }
    else if (sig == SIGTERM)
    {
        printf("\nReceived SIGTERM\n");
        printf("Termination signal handled.\n");
        printf("Program terminated.\n");
        exit(0);
    }
    else if (sig == SIGUSR1)
    {
        printf("\nReceived SIGUSR1\n");
        printf("User-defined signal handled.\n");
    }
}

int main()
{
    // Register signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGUSR1, signal_handler);

    printf("Signal handling program started.\n");
    printf("Process ID: %d\n", getpid());
    printf("Press Ctrl+C to generate SIGINT.\n");
    printf("Use another terminal to send SIGUSR1 or SIGTERM.\n");

    while (1)
    {
        printf("Program is running...\n");
        sleep(3);
    }

    return 0;
}
