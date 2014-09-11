#include <stdio.h>
#include "queue.h"

void init_queue(queue_t *q)
{
        q->first = 0;
        q->last = QUEUESIZE-1;
        q->count = 0;
}

void enqueue(queue_t *q, int x)
{
        if (q->count >= QUEUESIZE)
		printf("Warning: queue overflow enqueue x=%d\n",x);
        else {
                q->last = (q->last+1) % QUEUESIZE;
                q->q[ q->last ] = x;    
                q->count = q->count + 1;
        }
}

int dequeue(queue_t *q)
{
        int x;

        if (q->count <= 0) printf("Warning: empty queue dequeue.\n");
        else {
                x = q->q[ q->first ];
                q->first = (q->first+1) % QUEUESIZE;
                q->count = q->count - 1;
        }

        return(x);
}

int empty(queue_t *q)
{
        if (q->count <= 0) return (1);
        else return (0);
}

int length(queue_t *q)
{
        return q->count;
}

void print_queue(queue_t *q)
{
        int i,j;

        i=q->first; 
        
        while (i != q->last) {
                printf("%c ",q->q[i]);
                i = (i+1) % QUEUESIZE;
        }

        printf("%2d ",q->q[i]);
        printf("\n");
}
