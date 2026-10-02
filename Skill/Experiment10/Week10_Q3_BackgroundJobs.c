#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        printf("Background job started. PID = %d\n", getpid());
        sleep(5);
        printf("Background job completed.\n");
    }
    else
    {
        printf("Prompt returned immediately.\n");
        printf("Background job PID = %d\n", pid);

        sleep(2);

        if (waitpid(pid, NULL, WNOHANG) == 0)
            printf("Job is still running.\n");

        waitpid(pid, NULL, 0);
        printf("Background job finished.\n");
    }

    return 0;
}
