#include <stdio.h>
#include <stdlib.h>
#include "../include/commit.h"
#include "../include/string.h"

extern commit_queue *queue;

commit_t* create_commit(commit_type type, char* object)
{
    commit_t *c = (commit_t*) malloc(sizeof(commit_t));
    c->type = type;
    c->object_name = duplicate_string(object);
    c->ok = 0;
    return c;
}

void free_commit(commit_t* commit)
{
    if(commit)
    {
        if(commit->object_name)
            free(commit->object_name);
        free(commit);
    }
}

commit_queue* create_commit_queue()
{
    commit_queue *q = (commit_queue*) malloc(sizeof(commit_queue));
    q->size = 0;
    q->front = 0;
    q->rear = -1;
    return q;
}

void free_commit_queue(commit_queue* queue)
{
    if(queue)
    {
        free(queue);
    }
}

void enqueue_commit(commit_t* commit)
{
    if(queue->size != QUEUE_SIZE)
    {
        if(queue->rear == QUEUE_SIZE - 1)
        {
            queue->rear = -1;
        }
        queue->queue[++queue->rear] = commit;
        queue->size++;
    }
}

commit_t* dequeue_commit()
{
    if(queue->size == 0)
    {
        return NULL;
    }
    
    commit_t *c = queue->queue[queue->front++];
    if(queue->front == QUEUE_SIZE)
    {
        queue->front = 0;
    }
    queue->size--;
    return c;
}
