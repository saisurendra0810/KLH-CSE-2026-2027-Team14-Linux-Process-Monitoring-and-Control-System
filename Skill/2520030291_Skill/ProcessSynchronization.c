#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0)
    {
        printf("Process creation failed.\n");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child Process Started.\n");
        printf("Child PID: %d\n", getpid());

        sleep(2);

        printf("Child Process Completed.\n");
        return 10;
    }
    else
    {
        printf("Parent Process Started.\n");
        printf("Waiting for child process...\n");

        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            printf("Child process exited normally.\n");
            printf("Child exit status: %d\n",
                   WEXITSTATUS(status));
        }

        printf("Parent Process Completed.\n");
    }

    return 0;
}
