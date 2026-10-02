#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pipefd[2];
    pid_t p1, p2;

    pipe(pipefd);

    p1 = fork();

    if (p1 == 0)
    {
        int fd = open("input.txt", O_RDONLY);

        if (fd == -1)
        {
            perror("input.txt");
            return 1;
        }

        dup2(fd, STDIN_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);

        close(fd);
        close(pipefd[0]);
        close(pipefd[1]);

        execlp("cat", "cat", NULL);
        return 1;
    }

    p2 = fork();

    if (p2 == 0)
    {
        int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd == -1)
        {
            perror("output.txt");
            return 1;
        }

        dup2(pipefd[0], STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);

        close(fd);
        close(pipefd[0]);
        close(pipefd[1]);

        execlp("grep", "grep", "hello", NULL);
        return 1;
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    printf("Complex command completed.\n");

    return 0;
}
