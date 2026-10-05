#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define ITEMS 10

int buffer[BUFFER_SIZE];

int in = 0;
int out = 0;

sem_t empty;
sem_t full;

pthread_mutex_t buffer_mutex;

pthread_mutex_t resource1;
pthread_mutex_t resource2;

/* ---------- PRODUCER ---------- */

void *producer(void *arg)
{
    int i;

    for (i = 0; i < ITEMS; i++)
    {
        sem_wait(&empty);

        pthread_mutex_lock(&buffer_mutex);

        buffer[in] = i;

        printf("Produced: %d\n", i);

        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&buffer_mutex);

        sem_post(&full);
    }

    return NULL;
}

/* ---------- CONSUMER ---------- */

void *consumer(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEMS; i++)
    {
        sem_wait(&full);

        pthread_mutex_lock(&buffer_mutex);

        item = buffer[out];

        printf("Consumed: %d\n", item);

        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&buffer_mutex);

        sem_post(&empty);
    }

    return NULL;
}

/* ---------- PRODUCER-CONSUMER ---------- */

void producer_consumer()
{
    pthread_t producer_thread;
    pthread_t consumer_thread;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    pthread_mutex_init(&buffer_mutex, NULL);

    pthread_create(&producer_thread,
                   NULL,
                   producer,
                   NULL);

    pthread_create(&consumer_thread,
                   NULL,
                   consumer,
                   NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);

    pthread_mutex_destroy(&buffer_mutex);

    printf("Producer-Consumer completed.\n");
}

/* ---------- DEADLOCK PREVENTION ---------- */

void *thread1(void *arg)
{
    pthread_mutex_lock(&resource1);

    printf("Thread 1 acquired Resource 1\n");

    pthread_mutex_lock(&resource2);

    printf("Thread 1 acquired Resource 2\n");

    printf("Thread 1 is working...\n");

    pthread_mutex_unlock(&resource2);
    pthread_mutex_unlock(&resource1);

    return NULL;
}

void *thread2(void *arg)
{
    /*
     * Resource ordering:
     * Always acquire Resource 1 first,
     * then Resource 2.
     */

    pthread_mutex_lock(&resource1);

    printf("Thread 2 acquired Resource 1\n");

    pthread_mutex_lock(&resource2);

    printf("Thread 2 acquired Resource 2\n");

    printf("Thread 2 is working...\n");

    pthread_mutex_unlock(&resource2);
    pthread_mutex_unlock(&resource1);

    return NULL;
}

void deadlock_prevention()
{
    pthread_t t1, t2;

    pthread_mutex_init(&resource1, NULL);
    pthread_mutex_init(&resource2, NULL);

    pthread_create(&t1,
                   NULL,
                   thread1,
                   NULL);

    pthread_create(&t2,
                   NULL,
                   thread2,
                   NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&resource1);
    pthread_mutex_destroy(&resource2);

    printf("Deadlock prevented using resource ordering.\n");
}

/* ---------- MAIN ---------- */

int main()
{
    printf("========== EXPERIMENT 12 ==========\n");

    printf("\n--- Producer Consumer ---\n");
    producer_consumer();

    printf("\n--- Deadlock Prevention ---\n");
    deadlock_prevention();

    return 0;
}
