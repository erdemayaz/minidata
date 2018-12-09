#ifndef _COMMIT_H_
#define _COMMIT_H_

#define QUEUE_SIZE  32

#include <stdint.h>

typedef enum commit_type
{
    COMMIT_DROP_ENTITY
} commit_type;

typedef struct commit
{
    commit_type type;
    char *object_name;
    uint8_t ok;
} commit_t;

typedef struct commit_queue
{
    int size;
    commit_t *queue[QUEUE_SIZE];
    int rear;
    int front;
} commit_queue;

commit_t* create_commit(commit_type type, char* object);

void free_commit(commit_t* commit);

commit_queue* create_commit_queue();

void free_commit_queue(commit_queue* queue);

void enqueue_commit(commit_t* commit);

commit_t* dequeue_commit();

#endif
