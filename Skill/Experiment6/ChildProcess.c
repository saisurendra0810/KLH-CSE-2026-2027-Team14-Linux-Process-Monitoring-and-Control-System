#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        printf("Process creation failed.\n");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child Process Created.\n");
        printf("Executing ls command...\n");

        execlp("ls", "ls", "-l", NULL);

        printf("Execution failed.\n");
        return 1;
    }
    else
    {
        printf("Parent Process.\n");
        printf("Child PID: %d\n", pid);

        wait(NULL);

        printf("Child process completed.\n");
    }

    return 0;
}
