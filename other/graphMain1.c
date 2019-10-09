#include <stdlib.h>
#include <stdio.h>
#include "graphUtil.h"
#include "graph.h"
#include "queue.h"

Graph* initialize(short int V, char* filename){
	
	Graph *graph;
	Graph *graphComp;
	short int *compTrait;
	short int *component;
	
	graph = readGraph(filename,V);
	component = ccBfs(graph);
	
	compTrait = largestComp(component,graph->V);
	graphComp = extractComp(graph,component,compTrait[NUM],compTrait[SIZE]);
	printf("Graph initalized...\n");
	
	free(compTrait);
	free(component);
	deleteGraph(graph);
	return graphComp;
}

void switchRepr(Graph* graph, char format){
	adjList* list;
	short int** mat;
	short int V;
	
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
	short int i,j,k;
	short int **distMat,**next,**pathCount;
	Centrality *c;
	float temp1,temp2;
	FILE* f;
	
	distMat = initDistMatrix(adj->adjMat,adj->V);
	pathCount = copyMatrix(adj->adjMat,adj->V);
	
	printf("FW: Computing distances and path count...\n");
	for(k=0;k<adj->V;k++)
		for(i=0;i<adj->V;i++)
			for(j=i+1;j<adj->V;j++){
				if(distMat[i][j]>distMat[i][k]+distMat[k][j] && i!=k && k!=j){
					distMat[i][j] = distMat[i][k]+distMat[k][j];
					distMat[j][i] = distMat[i][j];
					pathCount[i][j] = pathCount[i][k]*pathCount[k][j];
					pathCount[j][i] = pathCount[i][j];
				}
				else if(distMat[i][j]==distMat[i][k]+distMat[k][j] && i!=k && k!=j){
					pathCount[i][j] = pathCount[i][j] + pathCount[i][k]*pathCount[k][j];
					pathCount[j][i] = pathCount[i][j];
				}
			}
			
//	printMatrix(pathCount,adj->V);
	c = ALLOC(Centrality);
	c->BCVal = createFloatArray(adj->V);
	c->V = adj->V;
	printf("FW: Computing centrality...\n");
	for(k=0;k<adj->V;k++){
		for(i=0;i<adj->V;i++)
			for(j=i+1;j<adj->V;j++){
				if(distMat[i][j]==distMat[i][k]+distMat[k][j] && i!=k && k!=j){
					temp1 = (float)(pathCount[i][k]*pathCount[k][j]);
					temp2 = (float)pathCount[i][j];
					c->BCVal[k] = c->BCVal[k] + (temp1/temp2);
				}
			}
		c->BCVal[k] = 2*c->BCVal[k]/((c->V - 1)*(c->V - 2));
	}

	freeMatrix(pathCount,adj->V);
	freeMatrix(distMat,adj->V);
	return c;
}

Centrality* bfsBCentrality(Graph *graph){
	short int current,src,i,j,k;
	adjNode *pCrawl;
	Queue* q;
	short int* visited;
	short int **distMat,**pathCount;
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
		pathCount[src][src] = 1;
		
		for(i=0;i<graph->V;i++){
			if(src!=i) distMat[src][i] = INFINITY;
		}
		
		while(!qIsEmpty(q)){
			current = dequeue(q);
			pCrawl = graph->adjLists[current].head;

			while(pCrawl!=NULL){
				
				if(visited[pCrawl->dest]==0){
					enqueue(q,pCrawl->dest);
					visited[pCrawl->dest] = 1;
				}
				
				if(distMat[src][pCrawl->dest] > distMat[src][current]+1){
					distMat[src][pCrawl->dest] = distMat[src][current]+1;
					pathCount[src][pCrawl->dest] = pathCount[src][current];
				}
				
				else if (distMat[src][pCrawl->dest] == distMat[src][current] + 1){
					pathCount[src][pCrawl->dest] = pathCount[src][pCrawl->dest] + pathCount[src][current];
				}
				
				pCrawl = pCrawl->next;
			}
		}
		pathCount[src][src]=0;
	}
	freeQueue(q);
	free(visited);
	
//	printMatrix(pathCount,graph->V);
	c = ALLOC(Centrality);
	c->BCVal = createFloatArray(graph->V);
	c->V = graph->V;
	printf("BFS: Computing centrality...\n");
	for(k=0;k<graph->V;k++){
		for(i=0;i<graph->V;i++)
			for(j=i+1;j<graph->V;j++)
				if(distMat[i][j]==distMat[i][k]+distMat[k][j] && i!=k && k!=j){
					temp1 = (float)(pathCount[i][k]*pathCount[k][j]);
					temp2 = (float)pathCount[i][j];
					c->BCVal[k] = c->BCVal[k] + (temp1/temp2);
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
		printMatrix((const short int **)graph->adjMat,graph->V);
	else
		printf("No graph to print\n");
}

void deleteGraph(Graph* graph){
	
	freeAdjList(graph->adjLists,graph->V);
	freeMatrix(graph->adjMat,graph->V);
	free(graph);
}