#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(1);
    }

    p1 = fork();

    if (p1 == -1)
    {
        perror("fork");
        exit(1);
    }

    if (p1 == 0)
    {
        // Child 1: execute ls -l
        close(pipefd[0]);

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        execlp("ls", "ls", "-l", (char *)NULL);

        perror("execlp ls");
        exit(1);
    }

    p2 = fork();

    if (p2 == -1)
    {
        perror("fork");
        exit(1);
    }

    if (p2 == 0)
    {
        // Child 2: execute grep ".c"
        close(pipefd[1]);

        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        execlp("grep", "grep", ".c", (char *)NULL);

        perror("execlp grep");
        exit(1);
    }

    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    return 0;
}
