#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    p1 = fork();

    if (p1 == 0)
    {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        execlp("ls", "ls", NULL);
        return 1;
    }

    p2 = fork();

    if (p2 == 0)
    {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        execlp("wc", "wc", "-l", NULL);
        return 1;
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    printf("Pipeline completed.\n");
    printf("Multiple jobs launched and monitored.\n");
    printf("Execution completed successfully.\n");

    return 0;
}
