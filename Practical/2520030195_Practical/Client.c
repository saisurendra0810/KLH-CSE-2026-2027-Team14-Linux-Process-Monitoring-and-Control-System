#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/calculator.sock"

int main() {
    int sock_fd;
    struct sockaddr_un addr;
    char buffer[100];

    // 1. Create socket
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sock_fd == -1) {
        perror("socket");
        exit(1);
    }

    // 2. Specify server socket
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    // 3. Connect to server
    connect(sock_fd,
            (struct sockaddr *)&addr,
            sizeof(addr));

    printf("Connected to server\n");

    // 4. Send data
    strcpy(buffer, "10 + 20");

    printf("Sending: %s\n", buffer);

    write(sock_fd, buffer, strlen(buffer) + 1);

    // 5. Receive result
    read(sock_fd, buffer, sizeof(buffer));

    printf("Server response: %s\n", buffer);

    close(sock_fd);

    return 0;
}

