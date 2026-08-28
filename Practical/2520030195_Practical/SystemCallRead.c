#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd;
    char buffer[100];
    int n;

    // Open the file in read-only mode
    fd = open("input.txt", O_RDONLY);

    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Read data from the file
    n = read(fd, buffer, sizeof(buffer) - 1);

    if (n == -1) {
        perror("Error reading file");
        close(fd);
        return 1;
    }

    // Add null character at the end
    buffer[n] = '\0';

    // Display the content
    printf("File content:\n%s", buffer);

    // Close the file
    close(fd);

    return 0;
}
