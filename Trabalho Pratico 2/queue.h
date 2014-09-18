#ifndef __QUEUE
#define __QUEUE

#define QUEUESIZE       1000

typedef struct {
        int q[QUEUESIZE+1];		/* body of queue */
        int first;              /* position of first element */
        int last;               /* position of last element */
        int count;              /* number of queued elements */
} queue_t;

void init_queue(queue_t *q);
void enqueue(queue_t *q, int x);
int dequeue(queue_t *q);

int length(queue_t *q);
int empty(queue_t *q);
void print_queue(queue_t *q);
#endif