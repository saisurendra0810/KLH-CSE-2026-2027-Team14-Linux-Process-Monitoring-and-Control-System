#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 1024

void copy_using_system_calls(const char *source, const char *destination)
{
    int src, dest;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead, bytesWritten;

    src = open(source, O_RDONLY);

    if (src == -1)
    {
        perror("Error opening source file");
        return;
    }

    dest = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (dest == -1)
    {
        perror("Error opening destination file");
        close(src);
        return;
    }

    while ((bytesRead = read(src, buffer, BUFFER_SIZE)) > 0)
    {
        bytesWritten = write(dest, buffer, bytesRead);

        if (bytesWritten != bytesRead)
        {
            perror("Error writing file");
            break;
        }
    }

    close(src);
    close(dest);

    printf("File copied using system calls.\n");
}

void copy_using_library(const char *source, const char *destination)
{
    FILE *src, *dest;
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    src = fopen(source, "r");
    dest = fopen(destination, "w");

    if (src == NULL || dest == NULL)
    {
        perror("Error opening file");
        return;
    }

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, src)) > 0)
    {
        fwrite(buffer, 1, bytesRead, dest);
    }

    fclose(src);
    fclose(dest);

    printf("File copied using standard library functions.\n");
}

void demonstrate_redirection()
{
    int inputFile, outputFile;

    inputFile = open("input.txt", O_RDONLY);

    if (inputFile == -1)
    {
        perror("Error opening input.txt");
        return;
    }

    outputFile = open("output.txt",
                      O_WRONLY | O_CREAT | O_TRUNC,
                      0644);

    if (outputFile == -1)
    {
        perror("Error opening output.txt");
        close(inputFile);
        return;
    }

    dup2(inputFile, STDIN_FILENO);
    dup2(outputFile, STDOUT_FILENO);

    close(inputFile);
    close(outputFile);

    printf("This output has been redirected to output.txt\n");
}

int main()
{
    int choice;

    printf("===== EXPERIMENT 9 =====\n");
    printf("1. File Copy\n");
    printf("2. I/O Redirection using dup2()\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        copy_using_system_calls("source.txt",
                                "destination.txt");

        copy_using_library("source.txt",
                           "destination_library.txt");

        printf("Both file-copy methods completed.\n");
    }
    else if (choice == 2)
    {
        demonstrate_redirection();
    }
    else
    {
        printf("Invalid choice.\n");
    }

    return 0;
}
