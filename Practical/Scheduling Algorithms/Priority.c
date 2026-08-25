#include <stdio.h>

int main()
{
    int n, i, j;
    int bt[20], priority[20];
    int ct[20], tat[20], wt[20];
    int pid[20];
    int temp;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter burst time and priority for each process:\n");

    for (i = 0; i < n; i++)
    {
        pid[i] = i + 1;

        printf("P%d Burst Time: ", i + 1);
        scanf("%d", &bt[i]);

        printf("P%d Priority: ", i + 1);
        scanf("%d", &priority[i]);
    }

    /* Sort according to priority */
    for (i = 0; i < n - 1; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (priority[i] > priority[j])
            {
                temp = priority[i];
                priority[i] = priority[j];
                priority[j] = temp;

                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;

                temp = pid[i];
                pid[i] = pid[j];
                pid[j] = temp;
            }
        }
    }

    ct[0] = bt[0];

    for (i = 1; i < n; i++)
    {
        ct[i] = ct[i - 1] + bt[i];
    }

    for (i = 0; i < n; i++)
    {
        tat[i] = ct[i];
        wt[i] = tat[i] - bt[i];
    }

    printf("\nProcess\tBT\tPriority\tCT\tTAT\tWT\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t\t%d\t%d\t%d\n",
               pid[i],
               bt[i],
               priority[i],
               ct[i],
               tat[i],
               wt[i]);
    }

    return 0;
}
