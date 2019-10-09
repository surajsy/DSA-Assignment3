#ifndef QUEUE_H
#define QUEUE_H

#ifndef ALLOC
#define ALLOC(x) (x*)malloc(sizeof(x))
#endif

#ifndef ALLOC_N
#define ALLOC_N(x,n) (x*)malloc(n*sizeof(x))
#endif

typedef struct q{
	int front;
	int rear;
	int size;
	int* queue;
}Queue;

Queue* createQueue(int N);

void enqueue(Queue* q,int num);

int dequeue(Queue *q);

int qIsFull(Queue* q);

int qIsEmpty(Queue* q);

void freeQueue(Queue* q);

#endif