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
        printf("Foreground job started. PID = %d\n", getpid());
        sleep(3);
        printf("Foreground job completed.\n");
    }
    else
    {
        printf("Target job identified: PID = %d\n", pid);
        printf("Waiting for foreground job...\n");

        waitpid(pid, NULL, 0);

        printf("Job state updated: Completed\n");
        printf("Terminal control returned to parent.\n");
    }

    return 0;
}
