#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signalHandler(int sig)
{
    printf("\nSignal %d received.\n", sig);
    printf("Handler executed successfully.\n");
}

int main()
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    printf("Signal handlers registered.\n");
    printf("PID = %d\n", getpid());
    printf("Press Ctrl+C to test SIGINT.\n");

    while (1)
        sleep(1);

    return 0;
}
