#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/calculator.sock"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[100];

    // Remove old socket file if it exists
    unlink(SOCKET_PATH);

    // 1. Create socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }

    // 2. Specify socket path
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    // 3. Bind socket to path
    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));

    // 4. Listen for clients
    listen(server_fd, 5);

    printf("Server started...\n");
    printf("Socket: %s\n", SOCKET_PATH);
    printf("Waiting for client...\n");

    // 5. Accept client
    client_fd = accept(server_fd, NULL, NULL);

    printf("Client connected!\n");

    // 6. Receive data
    read(client_fd, buffer, sizeof(buffer));
    printf("Client sent: %s\n", buffer);

    // 7. Send response
    write(client_fd, "30", 3);

    close(client_fd);
    close(server_fd);

    unlink(SOCKET_PATH);

    return 0;
}

