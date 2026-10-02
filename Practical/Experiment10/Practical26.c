#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

void inode_demo()
{
    FILE *file;
    struct stat st;

    file = fopen("original.txt", "w");

    if (file == NULL)
    {
        perror("Error creating file");
        return;
    }

    fprintf(file, "Hello from Experiment 10\n");
    fclose(file);

    if (stat("original.txt", &st) == -1)
    {
        perror("stat");
        return;
    }

    printf("\nOriginal file inode: %lu\n",
           (unsigned long)st.st_ino);

    if (link("original.txt", "hardlink.txt") == -1)
    {
        perror("Hard link");
    }
    else
    {
        printf("Hard link created.\n");
    }

    if (symlink("original.txt", "symlink.txt") == -1)
    {
        perror("Symbolic link");
    }
    else
    {
        printf("Symbolic link created.\n");
    }

    if (stat("hardlink.txt", &st) == 0)
    {
        printf("Hard link inode: %lu\n",
               (unsigned long)st.st_ino);
    }

    if (lstat("symlink.txt", &st) == 0)
    {
        printf("Symbolic link inode: %lu\n",
               (unsigned long)st.st_ino);
    }
}

void mmap_demo()
{
    int fd;
    struct stat st;
    char *data;

    fd = open("mmapfile.txt",
              O_RDWR | O_CREAT,
              0644);

    if (fd == -1)
    {
        perror("open");
        return;
    }

    ftruncate(fd, SIZE);

    if (fstat(fd, &st) == -1)
    {
        perror("fstat");
        close(fd);
        return;
    }

    data = mmap(NULL,
                SIZE,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                fd,
                0);

    if (data == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return;
    }

    strcpy(data, "Hello from memory mapped file I/O!");

    printf("\nData written using mmap():\n");
    printf("%s\n", data);

    msync(data, SIZE, MS_SYNC);

    munmap(data, SIZE);
    close(fd);

    printf("Memory-mapped file operation completed.\n");
}

int main()
{
    int choice;

    printf("===== EXPERIMENT 10 =====\n");
    printf("1. Inode and Link Demonstration\n");
    printf("2. Memory-Mapped File I/O\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        inode_demo();
    }
    else if (choice == 2)
    {
        mmap_demo();
    }
    else
    {
        printf("Invalid choice.\n");
    }

    return 0;
}
