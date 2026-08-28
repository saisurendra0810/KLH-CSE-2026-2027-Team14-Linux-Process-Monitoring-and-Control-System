#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main() {
    int fd;
    fd = open("test.txt", O_WRONLY | O_CREAT, 0644);

    // Check if file opened successfully
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    printf("File opened successfully.\n");
    printf("File descriptor: %d\n", fd);

    // Close the file
    close(fd);

    return 0;
}

