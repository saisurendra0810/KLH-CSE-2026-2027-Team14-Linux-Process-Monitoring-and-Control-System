#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void handle_signal(int sig)
{
if (sig == SIGINT)
printf("\nReceived SIGINT (Ctrl+C)\n");

else if (sig == SIGTERM)
printf("\nReceived SIGTERM - terminating gracefully...\n");

else if (sig == SIGUSR1)
printf("\nReceived SIGUSR1 - Application event 1\n");

else if (sig == SIGUSR2)
printf("\nReceived SIGUSR2 - Application event 2\n");

else if (sig == SIGALRM)
printf("\nReceived SIGALRM - Timer expired!\n");

else if (sig == SIGCHLD)
printf("\nReceived SIGCHLD - Child status changed\n");
}

int main()
{
pid_t pid;

signal(SIGINT, handle_signal);
signal(SIGTERM, handle_signal);
signal(SIGUSR1, handle_signal);
signal(SIGUSR2, handle_signal);
signal(SIGALRM, handle_signal);
signal(SIGCHLD, handle_signal);

printf("Parent Process Started\n");
printf("PID = %d\n", getpid());

printf("\nWaiting for signals...\n");

pid = fork();

if (pid == 0)
{
printf("Child Process Started. PID = %d\n", getpid());
sleep(5);
printf("Child Process Exiting...\n");
exit(0);
}
else
{
alarm(10);

while (1)
{
sleep(1);
}
}

return 0;
}
