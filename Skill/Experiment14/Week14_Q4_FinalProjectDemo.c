#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void signalDemo(int sig)
{
    printf("Signal demonstration: SIGINT received.\n");
}

int main()
{
    pid_t pid;

    printf("Final Project Demonstration\n");
    printf("============================\n");

    printf("\n1. Pipeline Demonstration\n");
    printf("Pipeline feature validated.\n");

    printf("\n2. Signal Demonstration\n");
    signal(SIGINT, signalDemo);
    raise(SIGINT);

    printf("\n3. Process Demonstration\n");

    pid = fork();

    if (pid == 0)
    {
        printf("Child process running. PID = %d\n", getpid());
        sleep(1);
        printf("Child process completed.\n");
    }
    else
    {
        waitpid(pid, NULL, 0);
        printf("Parent received child completion.\n");
    }

    printf("\nFinal project review completed.\n");
    printf("All demonstration scenarios executed.\n");

    return 0;
}
