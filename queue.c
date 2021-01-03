#include "queue.h"
#include <stdlib.h>

node_t *HEAD = NULL;
node_t *TAIL = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void enqueue(thread_args *args)
{
    //lock and unlock at end of critical section
    pthread_mutex_lock(&mutex);
    node_t *new = malloc(NODE_SIZE);
    new->args = args;
    new->next = NULL;
    if (TAIL == NULL)
    {
        HEAD = new;
    }
    else
    {
        TAIL->next = new;
    }
    TAIL = new;
    pthread_mutex_unlock(&mutex);
}

thread_args *dequeue()
{
    //lock and unlock at end of critical section
    pthread_mutex_lock(&mutex);
    if (HEAD == NULL)
    {
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    else
    {
        thread_args *result = HEAD->args;
        node_t *temp = HEAD;
        HEAD = HEAD->next;
        if (HEAD == NULL)
        {
            TAIL = NULL;
        }
        free(temp);
        pthread_mutex_unlock(&mutex);
        return result;
    }
}