#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t foregroundJob = -1;

void handleSIGINT(int sig)
{
    if (foregroundJob > 0)
    {
        printf("\nSIGINT received. Forwarding to foreground job.\n");
        kill(foregroundJob, SIGINT);
    }
    else
    {
        printf("\nShell process protected.\n");
    }
}

int main()
{
    signal(SIGINT, handleSIGINT);

    foregroundJob = fork();

    if (foregroundJob < 0)
    {
        perror("fork");
        return 1;
    }

    if (foregroundJob == 0)
    {
        signal(SIGINT, SIG_DFL);
        printf("Foreground job running. PID = %d\n", getpid());

        while (1)
            sleep(1);
    }
    else
    {
        printf("Press Ctrl+C to terminate the foreground job.\n");

        waitpid(foregroundJob, NULL, 0);
        foregroundJob = -1;

        printf("Foreground job terminated.\n");
        printf("Job state updated.\n");
    }

    return 0;
}
