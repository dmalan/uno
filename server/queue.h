#ifndef SERVER_QUEUE_HDR
#define SERVER_QUEUE_HDR

struct queue;

int queue_init(struct queue **q, int size);
int enqueue(struct queue *q, int fd);
int dequeue(struct queue *q);

#endif
