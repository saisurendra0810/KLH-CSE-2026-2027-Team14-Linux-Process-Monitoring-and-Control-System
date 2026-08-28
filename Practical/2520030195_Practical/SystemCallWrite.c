#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    char message[] = "Hello Linux! This is written using write() system call.\n";

    // Open or create the file for writing
    fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Write content to the file
    write(fd, message, sizeof(message) - 1);

    printf("Content written successfully.\n");

    // Close the file
    close(fd);

    return 0;
}
