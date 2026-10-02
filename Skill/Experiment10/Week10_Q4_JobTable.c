#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_JOBS 10

struct Job
{
    int id;
    pid_t pid;
    char state[20];
};

struct Job jobs[MAX_JOBS];
int jobCount = 0;

void addJob(pid_t pid)
{
    jobs[jobCount].id = jobCount + 1;
    jobs[jobCount].pid = pid;
    snprintf(jobs[jobCount].state, sizeof(jobs[jobCount].state), "Running");
    jobCount++;
}

void updateJobs()
{
    for (int i = 0; i < jobCount; i++)
    {
        if (waitpid(jobs[i].pid, NULL, WNOHANG) > 0)
            snprintf(jobs[i].state, sizeof(jobs[i].state), "Completed");
    }
}

void displayJobs()
{
    printf("\nJob Table\n");
    printf("-------------------------\n");
    printf("ID\tPID\tState\n");

    for (int i = 0; i < jobCount; i++)
        printf("%d\t%d\t%s\n",
               jobs[i].id, jobs[i].pid, jobs[i].state);
}

int main()
{
    pid_t p1, p2;

    p1 = fork();

    if (p1 == 0)
    {
        sleep(2);
        return 0;
    }

    addJob(p1);

    p2 = fork();

    if (p2 == 0)
    {
        sleep(4);
        return 0;
    }

    addJob(p2);

    displayJobs();

    sleep(3);
    updateJobs();

    printf("\nUpdated Job Table:\n");
    displayJobs();

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    return 0;
}
