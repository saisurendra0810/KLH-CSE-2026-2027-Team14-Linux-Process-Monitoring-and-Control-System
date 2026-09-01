#include <stdio.h>

int main()
{
    int n, i;
    int bt[20], remaining[20];
    int ct[20], tat[20], wt[20];
    int quantum;
    int time = 0;
    int completed = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter burst time for each process:\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d: ", i + 1);
        scanf("%d", &bt[i]);

        remaining[i] = bt[i];
        ct[i] = 0;
    }

    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    while (completed < n)
    {
        for (i = 0; i < n; i++)
        {
            if (remaining[i] > 0)
            {
                if (remaining[i] > quantum)
                {
                    time = time + quantum;
                    remaining[i] = remaining[i] - quantum;
                }
                else
                {
                    time = time + remaining[i];

                    remaining[i] = 0;

                    ct[i] = time;

                    completed++;
                }
            }
        }
    }

    for (i = 0; i < n; i++)
    {
        tat[i] = ct[i];
        wt[i] = tat[i] - bt[i];
    }

    printf("\nProcess\tBT\tCT\tTAT\tWT\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\n",
               i + 1,
               bt[i],
               ct[i],
               tat[i],
               wt[i]);
    }

    return 0;
}
