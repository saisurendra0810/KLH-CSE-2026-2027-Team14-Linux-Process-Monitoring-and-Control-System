#include <stdio.h>

int main()
{
    int n, i;
    int bt[20], ct[20], tat[20], wt[20];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter burst time for each process:\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d: ", i + 1);
        scanf("%d", &bt[i]);
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

    printf("\nProcess\tBT\tCT\tTAT\tWT\n");

    for (i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\n",
               i + 1, bt[i], ct[i], tat[i], wt[i]);
    }

    return 0;
}
