#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("Job started. PID = %d\n", getpid());
        raise(SIGSTOP);

        printf("Job resumed using SIGCONT.\n");
        sleep(2);
        printf("Job completed.\n");
    }
    else
    {
        waitpid(pid, &status, WUNTRACED);

        if (WIFSTOPPED(status))
        {
            printf("Job status: Stopped\n");
            printf("Sending SIGCONT...\n");
            kill(pid, SIGCONT);
        }

        waitpid(pid, NULL, 0);
        printf("Job status: Completed\n");
    }

    return 0;
}
