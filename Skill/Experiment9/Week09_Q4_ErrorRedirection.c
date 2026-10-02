#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    int saved_stderr;

    fd = open("error.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1)
    {
        perror("error.txt");
        return 1;
    }

    saved_stderr = dup(STDERR_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);

    fprintf(stderr, "This is an error message.\n");
    fprintf(stderr, "Error output captured successfully.\n");

    fflush(stderr);
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);

    printf("Error stream restored.\n");

    return 0;
}
