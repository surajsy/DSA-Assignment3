#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue* createQueue(int N){
	Queue *q;
	
	q = ALLOC(Queue);
	q->queue = ALLOC_N(int,N);
	q->front = 0;
	q->rear = N-1;
	q->size = N;
	return q;
}

void enqueue(Queue* q,int num){
	if(!qIsFull(q)){
		q->rear = (q->rear + 1) % (q->size);
		q->queue[q->rear] = num;
	}
	
	else{
		printf("Queue is full\n");
	}
}

int dequeue(Queue *q){
	int num;
	if(!qIsEmpty(q)){
		num = q->queue[q->front];
		q->front = (q->front + 1) % (q->size);
	}
	return num;
}

void freeQueue(Queue* q){
	free(q->queue);
	free(q);
}

int qIsFull(Queue* q){
	if(q==NULL){
		printf("Queue pointer is NULL\n");
		exit(0);
	}
		
	if((q->rear + 2) % q->size == q->front)
		return 1;
	else 
		return 0;
}

int qIsEmpty(Queue* q){
	if(q==NULL){
		printf("Queue pointer is NULL\n");
		exit(0);
	}
	
	if((q->rear + 1) % q->size == q->front)
		return 1;
	else 
		return 0;
}