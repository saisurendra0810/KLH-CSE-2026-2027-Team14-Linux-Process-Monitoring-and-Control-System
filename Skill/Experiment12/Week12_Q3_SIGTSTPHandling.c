#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t jobPID;

void handleSIGTSTP(int sig)
{
    printf("\nSIGTSTP received.\n");

    if (jobPID > 0)
        kill(jobPID, SIGSTOP);
}

int main()
{
    int status;

    signal(SIGTSTP, handleSIGTSTP);

    jobPID = fork();

    if (jobPID < 0)
    {
        perror("fork");
        return 1;
    }

    if (jobPID == 0)
    {
        printf("Process running. PID = %d\n", getpid());

        while (1)
        {
            printf("Process working...\n");
            sleep(2);
        }
    }
    else
    {
        printf("Press Ctrl+Z to suspend the process.\n");

        waitpid(jobPID, &status, WUNTRACED);

        if (WIFSTOPPED(status))
        {
            printf("Job Table: Job is Stopped.\n");
            printf("Resuming job with SIGCONT...\n");

            kill(jobPID, SIGCONT);
            sleep(1);

            printf("Job Table: Job is Running.\n");

            kill(jobPID, SIGTERM);
            waitpid(jobPID, NULL, 0);

            printf("Job completed.\n");
        }
    }

    return 0;
}
