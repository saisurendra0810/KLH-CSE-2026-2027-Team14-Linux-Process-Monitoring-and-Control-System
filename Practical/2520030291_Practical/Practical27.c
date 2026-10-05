#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ITERATIONS 100000

int counter = 0;

pthread_mutex_t mutex;

void *increment_without_mutex(void *arg)
{
    int i;

    for (i = 0; i < ITERATIONS; i++)
    {
        counter++;
    }

    return NULL;
}

void *increment_with_mutex(void *arg)
{
    int i;

    for (i = 0; i < ITERATIONS; i++)
    {
        pthread_mutex_lock(&mutex);

        counter++;

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void run_race_condition()
{
    pthread_t threads[NUM_THREADS];
    int i;

    counter = 0;

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i],
                       NULL,
                       increment_without_mutex,
                       NULL);
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\n--- Race Condition ---\n");
    printf("Expected Counter: %d\n",
           NUM_THREADS * ITERATIONS);

    printf("Actual Counter: %d\n",
           counter);
}

void run_with_mutex()
{
    pthread_t threads[NUM_THREADS];
    int i;

    counter = 0;

    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i],
                       NULL,
                       increment_with_mutex,
                       NULL);
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\n--- With Mutex ---\n");
    printf("Expected Counter: %d\n",
           NUM_THREADS * ITERATIONS);

    printf("Actual Counter: %d\n",
           counter);

    pthread_mutex_destroy(&mutex);
}

int main()
{
    printf("===== EXPERIMENT 11 =====\n");

    run_race_condition();

    run_with_mutex();

    return 0;
}
