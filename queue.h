#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include "server.h"


typedef struct node
{
    struct node *next;
    thread_args *args;
} node_t;

#define NODE_SIZE (sizeof(node_t))

void enqueue(thread_args *args);
thread_args *dequeue();
#endif