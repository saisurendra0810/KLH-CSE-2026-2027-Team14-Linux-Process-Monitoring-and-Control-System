#include <stdio.h>
#include <unistd.h>

struct Job
{
    int id;
    pid_t pid;
    char state[20];
};

int main()
{
    struct Job jobs[3];
    int count = 3;

    for (int i = 0; i < count; i++)
    {
        jobs[i].id = i + 1;
        jobs[i].pid = 1000 + i;
        snprintf(jobs[i].state, sizeof(jobs[i].state), "Running");
    }

    printf("Active Jobs\n");
    printf("-------------------------\n");
    printf("Job ID\tPID\tStatus\n");

    for (int i = 0; i < count; i++)
        printf("[%d]\t%d\t%s\n",
               jobs[i].id, jobs[i].pid, jobs[i].state);

    printf("\nJob listing completed successfully.\n");

    return 0;
}
