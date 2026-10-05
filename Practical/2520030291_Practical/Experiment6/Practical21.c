#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFO1 "client_to_server"
#define FIFO2 "server_to_client"

int main()
{
    int fd1, fd2;
    char message[100];
    char response[100];

    printf("Enter message for server: ");
    fgets(message, sizeof(message), stdin);

    // Remove newline
    message[strcspn(message, "\n")] = '\0';

    // Open FIFO for writing
    fd1 = open(FIFO1, O_WRONLY);

    // Send message to server
    write(fd1, message, strlen(message) + 1);

    close(fd1);

    // Open FIFO for reading
    fd2 = open(FIFO2, O_RDONLY);

    // Read server response
    read(fd2, response, sizeof(response));

    printf("Server: %s\n", response);

    close(fd2);

    return 0;
}
