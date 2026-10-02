#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main()
{
    pid_t p1, p2;
    pid_t groupID;

    p1 = fork();

    if (p1 == 0)
    {
        setpgid(0, 0);
        printf("Process 1: PID=%d PGID=%d\n", getpid(), getpgrp());
        sleep(5);
        return 0;
    }

    p2 = fork();

    if (p2 == 0)
    {
        setpgid(0, p1);
        printf("Process 2: PID=%d PGID=%d\n", getpid(), getpgrp());
        sleep(5);
        return 0;
    }

    sleep(1);

    setpgid(p1, p1);
    setpgid(p2, p1);

    groupID = p1;

    printf("Process group created.\n");
    printf("Group ID = %d\n", groupID);

    printf("Sending signal to complete process group...\n");
    kill(-groupID, SIGTERM);

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    printf("Process group completed.\n");
    printf("Terminal control can now be restored.\n");

    return 0;
}
