/**
 * 
 * An int queue
 */
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "queue.h"

/*
 * if we really cared to, we could do something like:
 *
 * struct queue
 * {
 *     int type;
 *     union 
 *     {
 *         struct array_queue *aq
 *         struct list_queue *lq
 *     } q;
 * };
 *
 * A size of 0 could even be used to signify list_queue.
 * But for now we'll just assume that we can allocate a static array
 *
 */

// TODO: WTF is a futex
struct queue
{
    int *queue;
    int size;
    int offset;
    int max_size;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
};


int
queue_init(struct queue **q, int size)
{
    int err;
    if (q == NULL || size == 0)
        return -EINVAL;
    struct queue *new_q;
    new_q = malloc(sizeof (struct queue));
    if (new_q == NULL)
        return -ENOMEM;
    new_q->queue = malloc(sizeof (int) * size);
    if (new_q->queue == NULL)
    {
        err = -ENOMEM;
        goto e1;
    }
    if ((err = pthread_mutex_init(&new_q->mutex, NULL)))
        goto e2;
    if ((err = pthread_cond_init(&new_q->cv, NULL)))
        goto e3;
    new_q->max_size = size;
    new_q->size = 0;
    new_q->offset = 0;
    *q = new_q;
    return 0;

e3: pthread_mutex_destroy(&new_q->mutex);
e2: free(new_q->queue);
e1: free(new_q);
    return err;
}

// does *not* return an error if one happens
void
queue_destroy(struct queue *q)
{
    if (q == NULL)
        return;
    free(q);
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->cv);
    if (q->queue != NULL)
        free(q->queue);
}

int
enqueue(struct queue *q, int fd)
{
    #ifdef ERR_CHK
        if (q == NULL)
            return -EINVAL;
    #endif

    pthread_mutex_lock(&q->mutex);
    if (q->size == q->max_size)
    {
        pthread_mutex_unlock(&q->mutex);
        return -EAGAIN;
    }

    q->queue[(q->size + q->offset) % q->max_size] = fd;
    q->size++;
    pthread_cond_signal(&q->cv);
    pthread_mutex_unlock(&q->mutex);
    return 0;
}

int
dequeue(struct queue *q)
{
    #ifdef ERR_CHK
    if (q == NULL || q->queue == NULL)
        return -EINVAL;
    #endif
    
    pthread_mutex_lock(&q->mutex);
    while (q->size == 0)
        pthread_cond_wait(&q->cv, &q->mutex);

    int ret = q->queue[q->offset++];
    q->offset %= q->max_size;
    q->size--;
    pthread_mutex_unlock(&q->mutex);
    return ret;
}
