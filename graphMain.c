#include <stdlib.h>
#include <stdio.h>
#include "graphUtil.h"
#include "graph.h"
#include "queue.h"

Graph* initialize(int V, char* filename){
	
	Graph *graph;
	Graph *graphComp;
	int *compTrait;
	int *component;
	
	graph = readGraph(filename,V);
	component = ccBfs(graph);
	
	compTrait = largestComp(component,graph->V);
	graphComp = extractComp(graph,component,compTrait[NUM],compTrait[SIZE]);
	printf("Graph initalized...\n");
	printf("Vc = %d\n",graph->V);
	
	free(compTrait);
	free(component);
	deleteGraph(graph);
	return graphComp;
}

void switchRepr(Graph* graph, char format){
	adjList* list;
	int* mat;
	int V;
	
	if(graph == NULL){
		printf("Graph is NULL");
		exit(0);
	}
	
	switch(format){
		case 'l':
			list = mat2AdjList(graph);
			freeMatrix(graph->adjMat,graph->V);
			graph->adjLists = list;
			graph->adjMat = NULL;
			break;
			
		case 'm':
			mat = adjList2Mat(graph);
			freeAdjList(graph->adjLists,graph->V);
			graph->adjMat = mat;
			graph->adjLists = NULL;
			break;
			
		default:
			printf("Invalid Graph format\n");
			exit(0);
	}
	printf("Changed graph representation...\n");
}

Centrality* fwBCentrality(Graph* adj){
	int i,j,k;
	int ij,ik,kj;
	int *distMat,*next,*pathCount;
	Centrality *c;
	float temp1,temp2;
	FILE* f;
	
	distMat = initDistMatrix(adj->adjMat,adj->V);
	pathCount = copyMatrix(adj->adjMat,adj->V);
	
	printf("FW: Computing distances and path count...\n");
	for(k=0;k<adj->V;k++)
		for(i=0;i<adj->V;i++){
			ik = ind(i,k,adj->V);
			for(j=0;j<i;j++){
				ij = ind(i,j,adj->V);
				kj = ind(k,j,adj->V);
				if(distMat[ij]>distMat[ik]+distMat[kj] && i!=k && k!=j){
					distMat[ij] = distMat[ik]+distMat[kj];
					pathCount[ij] = pathCount[ik]*pathCount[kj];
				}
				else if(distMat[ij]==distMat[ik]+distMat[kj] && i!=k && k!=j){
					pathCount[ij] = pathCount[ij] + pathCount[ik]*pathCount[kj];
				}
			}
		}
			
//	printMatrix(pathCount,adj->V);
	c = ALLOC(Centrality);
	c->BCVal = createFloatArray(adj->V);
	c->V = adj->V;
	printf("FW: Computing centrality...\n");
	for(k=0;k<adj->V;k++){
		for(i=0;i<adj->V;i++){
			ik = ind(i,k,adj->V);
			for(j=0;j<i;j++){
				ij = ind(i,j,adj->V);
				kj = ind(k,j,adj->V);
				
				if(distMat[ij]==distMat[ik]+distMat[kj] && i!=k && k!=j){
					temp1 = (float)(pathCount[ik]*pathCount[kj]);
					temp2 = (float)pathCount[ij];
					c->BCVal[k] = c->BCVal[k] + (temp1/temp2);
				}
			}
		}
		c->BCVal[k] = 2*c->BCVal[k]/((c->V - 1)*(c->V - 2));
	}

	freeMatrix(pathCount,adj->V);
	freeMatrix(distMat,adj->V);
	return c;
}

Centrality* bfsBCentrality(Graph *graph){
	int current,src,i,j,k;
	int sd,sc,ss;
	adjNode *pCrawl;
	Queue* q;
	int* visited;
	int *distMat,*pathCount;
	Centrality* c;
	float temp1,temp2;
	FILE* f;
	
	q = createQueue(graph->V);
	visited = createIntArray(graph->V);
	distMat = createMatrix(graph->V);
	pathCount = createMatrix(graph->V);
	
	printf("BFS: Computing distances and path count...\n");
	for(src=0;src < graph->V; src++){
		
		for(i=0;i<graph->V;i++)
			visited[i]=0;

		enqueue(q,src);
		visited[src] = 1;
		ss = ind(src,src,graph->V);
		pathCount[ss] = 1;
		
		for(i=0;i<graph->V;i++){
			if(src!=i) distMat[ind(src,i,graph->V)] = INFINITY;
		}
		
		while(!qIsEmpty(q)){
			current = dequeue(q);
			pCrawl = graph->adjLists[current].head;
			sc = ind(src,current,graph->V);

			while(pCrawl!=NULL){
				
				if(visited[pCrawl->dest]==0){
					enqueue(q,pCrawl->dest);
					visited[pCrawl->dest] = 1;
				}
				
				sd = ind(src,pCrawl->dest,graph->V);
				
				if(distMat[sd] > distMat[sc]+1){
					distMat[sd] = distMat[sc]+1;
					pathCount[sd] = pathCount[sc];
				}
				
				else if (distMat[sd] == distMat[sc]+1){
					pathCount[sd] = pathCount[sd] + pathCount[sc];
				}
				
				pCrawl = pCrawl->next;
			}
		}
		pathCount[ss]=0;
	}
	freeQueue(q);
	free(visited);
	
//	printMatrix(pathCount,graph->V);
	c = ALLOC(Centrality);
	c->BCVal = createFloatArray(graph->V);
	c->V = graph->V;
	printf("BFS: Computing centrality...\n");
	for(k=0;k<graph->V;k++){
		for(i=0;i<graph->V;i++){
			sc = ind(i,k,graph->V);
			for(j=0;j<i;j++){
				ss = ind(i,j,graph->V);
				sd = ind(k,j,graph->V);
				if(distMat[ss]==distMat[sc]+distMat[sd] && i!=k && k!=j){
					temp1 = (float)(pathCount[sc]*pathCount[sd]);
					temp2 = (float)pathCount[ss];
					c->BCVal[k] = c->BCVal[k] + (temp1/temp2);
				}
			}
		}
		c->BCVal[k] = 2*c->BCVal[k]/((c->V - 1)*(c->V - 2));
	}

	freeMatrix(pathCount,graph->V);
	freeMatrix(distMat,graph->V);
	return c;
}

void printGraph(const Graph *graph){
	if(graph==NULL){
		printf("Graph is NULL\n");
		exit(0);
	}
	
	if(graph->adjLists != NULL)
		printAdjList((const adjList *)graph->adjLists,graph->V);
	else if(graph->adjMat != NULL)
		printMatrix((const int *)graph->adjMat,graph->V);
	else
		printf("No graph to print\n");
}

void deleteGraph(Graph* graph){
	
	freeAdjList(graph->adjLists,graph->V);
	freeMatrix(graph->adjMat,graph->V);
	free(graph);
}