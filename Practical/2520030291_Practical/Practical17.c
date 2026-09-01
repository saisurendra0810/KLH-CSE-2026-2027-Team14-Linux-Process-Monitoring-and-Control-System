#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void create_children_wait()
{
    int i;
    pid_t pid;
    int status;

    printf("\n===== USING wait() =====\n");

    for (i = 0; i < 3; i++)
    {
        pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0)
        {
            printf("Child %d: PID = %d, Parent PID = %d\n",
                   i + 1, getpid(), getppid());

            sleep(2 + i);

            printf("Child %d: Finished\n", i + 1);
            exit(10 + i);
        }
    }

    /* Parent waits for any child */
    for (i = 0; i < 3; i++)
    {
        pid = wait(&status);

        if (pid > 0)
        {
            if (WIFEXITED(status))
            {
                printf("Parent: Child PID %d completed with status %d\n",
                       pid, WEXITSTATUS(status));
            }
        }
    }

    printf("Parent: All children completed using wait().\n");
}

void create_children_waitpid()
{
    int i;
    pid_t pid;
    pid_t child_pids[3];
    int status;

    printf("\n===== USING waitpid() =====\n");

    for (i = 0; i < 3; i++)
    {
        pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0)
        {
            printf("Child %d: PID = %d, Parent PID = %d\n",
                   i + 1, getpid(), getppid());

            sleep(2 + i);

            printf("Child %d: Finished\n", i + 1);
            exit(20 + i);
        }

        child_pids[i] = pid;
    }

    /*
     * Parent waits for a specific child
     * using waitpid().
     */
    for (i = 0; i < 3; i++)
    {
        pid = waitpid(child_pids[i], &status, 0);

        if (pid > 0)
        {
            if (WIFEXITED(status))
            {
                printf("Parent: Specific child PID %d completed "
                       "with status %d\n",
                       pid, WEXITSTATUS(status));
            }
        }
    }

    printf("Parent: All children completed using waitpid().\n");
}

void create_zombie()
{
    pid_t pid;

    printf("\n===== ZOMBIE PROCESS DEMONSTRATION =====\n");

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0)
    {
        printf("Zombie child: PID = %d\n", getpid());
        printf("Zombie child: Exiting immediately...\n");

        exit(0);
    }
    else
    {
        printf("Parent: PID = %d\n", getpid());
        printf("Parent: Child PID = %d\n", pid);

        printf("Parent: Sleeping for 10 seconds.\n");
        printf("During this time, the child becomes a ZOMBIE.\n");

        sleep(10);

        printf("Parent: Now calling wait() to remove zombie.\n");

        wait(NULL);

        printf("Parent: Zombie process has been cleaned up.\n");
    }
}

void eliminate_zombie()
{
    pid_t pid;
    int status;

    printf("\n===== ELIMINATING ZOMBIE USING waitpid() =====\n");

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0)
    {
        printf("Child: PID = %d\n", getpid());
        printf("Child: Doing some work...\n");

        sleep(2);

        printf("Child: Exiting...\n");
        exit(0);
    }
    else
    {
        printf("Parent: PID = %d\n", getpid());
        printf("Parent: Waiting for child PID %d using waitpid().\n",
               pid);

        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            printf("Parent: Child terminated normally.\n");
            printf("Parent: Exit status = %d\n",
                   WEXITSTATUS(status));
        }

        printf("Parent: No zombie process remains.\n");
    }
}

int main()
{
    printf("========================================\n");
    printf(" PROCESS SYNCHRONIZATION DEMONSTRATION\n");
    printf("========================================\n");

    /* Part 1: wait() */
    create_children_wait();

    /* Part 2: waitpid() */
    create_children_waitpid();

    /* Part 3: Create zombie */
    create_zombie();

    /* Part 4: Remove zombie properly */
    eliminate_zombie();

    printf("\n========================================\n");
    printf("Program completed successfully.\n");
    printf("========================================\n");

    return 0;
}
