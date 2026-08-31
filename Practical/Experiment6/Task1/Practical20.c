#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define FIFO1 "client_to_server"
#define FIFO2 "server_to_client"

int main()
{
    int fd1, fd2;
    char message[100];
    char response[200];
    ssize_t bytes_read;

    // Create FIFOs
    if (mkfifo(FIFO1, 0666) == -1 && errno != EEXIST)
    {
        perror("mkfifo client_to_server");
        return 1;
    }

    if (mkfifo(FIFO2, 0666) == -1 && errno != EEXIST)
    {
        perror("mkfifo server_to_client");
        return 1;
    }

    printf("Server started...\n");
    printf("Waiting for client message...\n");

    // Open FIFO for reading
    fd1 = open(FIFO1, O_RDONLY);

    if (fd1 == -1)
    {
        perror("open FIFO1");
        return 1;
    }

    // Read message
    bytes_read = read(fd1, message, sizeof(message) - 1);

    if (bytes_read == -1)
    {
        perror("read");
        close(fd1);
        return 1;
    }

    // Add null character
    message[bytes_read] = '\0';

    printf("Client: %s\n", message);

    close(fd1);

    // Process message
    snprintf(response, sizeof(response),
             "Server received and processed: %s", message);

    // Open FIFO for writing
    fd2 = open(FIFO2, O_WRONLY);

    if (fd2 == -1)
    {
        perror("open FIFO2");
        return 1;
    }

    // Send response
    write(fd2, response, strlen(response) + 1);

    printf("Response sent to client.\n");

    close(fd2);

    // Remove FIFOs
    unlink(FIFO1);
    unlink(FIFO2);

    return 0;
}
