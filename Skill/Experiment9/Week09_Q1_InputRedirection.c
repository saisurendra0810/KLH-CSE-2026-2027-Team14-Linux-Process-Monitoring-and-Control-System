#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    int saved_stdin;
    char buffer[200];

    fd = open("input.txt", O_RDONLY);

    if (fd == -1)
    {
        perror("input.txt");
        return 1;
    }

    saved_stdin = dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO);
    close(fd);

    printf("Reading from input.txt:\n");

    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
        printf("%s", buffer);

    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);

    return 0;
}
