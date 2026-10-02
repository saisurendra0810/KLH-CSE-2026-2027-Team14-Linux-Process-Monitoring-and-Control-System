#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig)
{
    printf("\nSignal %d delivered to PID %d\n", sig, getpid());
}

int main()
{
    sigset_t mask;

    signal(SIGUSR1, handler);

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    printf("PID = %d\n", getpid());
    printf("Testing signal delivery and signal mask.\n");

    printf("Blocking SIGUSR1...\n");
    sigprocmask(SIG_BLOCK, &mask, NULL);

    printf("SIGUSR1 is currently blocked.\n");
    sleep(2);

    printf("Unblocking SIGUSR1...\n");
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    printf("Signal mask restored.\n");
    printf("Process Group ID = %d\n", getpgrp());
    printf("Terminal signal concepts demonstrated.\n");

    return 0;
}
