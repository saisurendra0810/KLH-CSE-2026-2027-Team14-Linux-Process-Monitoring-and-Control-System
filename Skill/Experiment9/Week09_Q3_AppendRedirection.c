#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    int saved_stdout;

    fd = open("append.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);

    if (fd == -1)
    {
        perror("append.txt");
        return 1;
    }

    saved_stdout = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    close(fd);

    printf("New data appended to the file.\n");

    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    printf("Append operation completed.\n");

    return 0;
}
