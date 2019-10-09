#include <stdio.h>
#include <stdlib.h>
#include "graphUtil.h"
#include "queue.h"

short int* createIntArray(short int V){
	short int* array;
	short int i;
	array = ALLOC_N(short int,V);
	for(i=0;i<V;i++)
		array[i]=0;
	return array;
}

float* createFloatArray(short int V){
	float* array;
	short int i;
	array = ALLOC_N(float,V);
	for(i=0;i<V;i++)
		array[i]=0;
	return array;
}

short int** createMatrix(short int V){
	short int i,v;
	short int** adjMatrix;
	
	adjMatrix = ALLOC_N(short int*,V);
	for(i=0;i<V;i++)
		adjMatrix[i]=ALLOC_N(short int,V);
	
	for(v=0;v<V;v++)
		for(i=v;i<V;i++){
			adjMatrix[v][i] = 0;
			adjMatrix[i][v] = 0;
		}
	return adjMatrix;
}

adjNode* newAdjListNode(short int dest){
	adjNode* newNode;
	newNode = ALLOC(adjNode);
	newNode->dest = dest;
	newNode->next = NULL;
	return newNode;
}

adjList* createAdjList(short int V){
	adjList* adjLists;
	short int i;
	
	adjLists = ALLOC_N(adjList,V);
	for(i=0;i<V;i++){
		adjLists[i].head = NULL;
	}
	
	return adjLists;
}

Graph* createGraph(short int V, char format){
	short int i;
	Graph* graph = ALLOC(Graph);
	graph->V = V;
	
	if(format == 'l'){
		graph->adjLists = createAdjList(V);
		graph->adjMat = NULL;
	}
	
	else if(format == 'm'){
		graph->adjLists = NULL;
		graph->adjMat = NULL;
	}
	
	else{
		printf("Invalid graph format...exiting\n");
		exit(0);
	}
	
	return graph;
}

/*short int** initPathMatrix(short int** adj, short int V){
	short int v,w;
	short int** mat;
	
	mat = createMatrix(V);
	
	for(v=0;v<adj->V;v++)
		for(w=0;w<V;w++)
			if(adj[v][w]==1)
				mat[v][w]=w+1;
			else if (v==w)
				mat[v][w]=v+1;
	
	return mat;
}*/

short int** initDistMatrix(short int** adj, short int V){
	short int v,w;
	short int** mat;
	
	mat = createMatrix(V);
	
	for(v=0;v<V;v++)
		for(w=0;w<V;w++)
			if(adj[v][w]==1)
				mat[v][w]=1;
			else if(v!=w)
				mat[v][w]=INFINITY;
	
	return mat;
}

short int** copyMatrix(short int** adj, short int V){
	short int v,w;
	short int** mat;
	
	mat = createMatrix(V);
	
	for(v=0;v<V;v++)
		for(w=0;w<V;w++)
			mat[v][w]=adj[v][w];
			
	return mat;
}

void addEdge(Graph* graph, short int src, short int dest){
	adjNode *newNode;
	
	newNode = newAdjListNode(dest);
	newNode->next = graph->adjLists[src].head;
	graph->adjLists[src].head = newNode;
	
	newNode = newAdjListNode(src);
	newNode->next = graph->adjLists[dest].head;
	graph->adjLists[dest].head = newNode;
}

/*adjNode* copyAdjList(adjNode* head){
	adjNode *newHead,*newNode,*pCrawl;
	
	pCrawl = head;
	newHead = NULL;
	
	while(pCrawl!=NULL){
		if(newHead==NULL){
			newHead = newAdjListNode(pCrawl->dest);
			pCrawl = pCrawl->next;
		}
		else{
			newNode = newHead;
			while(newNode->next!=NULL)
				newNode = newNode->next;
			newNode->next = newAdjListNode(pCrawl->dest);
			pCrawl = pCrawl->next;
		}
	}
	
	return newHead;
}*/

short int* ccBfs(Graph *graph){
	short int v,v0,count;
	adjNode *pCrawl;
	Queue* q;
	short int *component;
	
	q = createQueue(graph->V);
	component = createIntArray(graph->V);
	
	count = 0;
	for(v=0;v < graph->V; v++){
		enqueue(q,v);
		if(component[v]==0)
			component[v] = ++count;
			
		while(!qIsEmpty(q)){
			v0 = dequeue(q);
			pCrawl = graph->adjLists[v0].head;

			while(pCrawl!=NULL){
				
				if(component[pCrawl->dest]==0){
					enqueue(q,pCrawl->dest);
					component[pCrawl->dest] = count;
				}
				
				pCrawl = pCrawl->next;
			}
		}
	}
	freeQueue(q);
	return component;
}

short int findMaxInd(short int* array,short int N){
	short int i,max,index;
	
	max=-1;
	for(i=0;i<N;i++){
		if(array[i]>max){
			max = array[i];
			index = i;
		}
	}
	return index;
}

short int* largestComp(short int* array,short int N){
	short int nComp,i;
	short int* compSize;
	short int* comp;
	
	nComp = array[findMaxInd(array,N)];
	compSize = ALLOC_N(short int,nComp);
	
	for(i=0;i<nComp;i++)
		compSize[i] = 0;
		
	for(i=0;i<N;i++){
		compSize[array[i]-1]++;
	}

	comp = ALLOC_N(short int,2);
	comp[0] = findMaxInd(compSize,nComp)+1;
	comp[1] = compSize[comp[0]-1];
	
	free(compSize);
	return comp;
}

Graph* extractComp(Graph* graph,short int *component, short int compN,short int compSize){
	short int **adjMatGraph;
	Graph *adjMatComp;
	short int v,w,Vg,Vc;
	short int m,n;
	
	Vg = graph->V;
	Vc = compSize;
	adjMatGraph = adjList2Mat(graph);
	
	adjMatComp = createGraph(Vc,'m');
	
	if(Vc==Vg){
		adjMatComp->adjMat = adjMatGraph;
		return adjMatComp;
	}
	
	adjMatComp->adjMat = createMatrix(Vc);

	m=-1;
	n=0;
	
	for(v=0;v<Vg;v++){
		if(component[v] == compN){
			m++;
			n=m;
		
			for(w=v;w<Vg;w++){
				if(component[w]==compN){
					adjMatComp->adjMat[m][n]=adjMatGraph[v][w];
					adjMatComp->adjMat[n][m]=adjMatGraph[w][v];
					n++;
				}
			}
		}
	}

	freeMatrix(adjMatGraph,Vg);

	return adjMatComp;
}

short int** adjList2Mat(Graph* graph){
	short int** adjMatrix;
	short int v,i;
	adjNode* pCrawl;
	
	adjMatrix = createMatrix(graph->V);
	
	for(v=0;v<graph->V;v++){
		pCrawl = graph->adjLists[v].head;
		while(pCrawl!=NULL){
			adjMatrix[v][pCrawl->dest]=1;
			pCrawl = pCrawl->next;
		}
	}
	
	return adjMatrix;
}

adjList* mat2AdjList(Graph* adj){
	adjList* list;
	short int v,w;
	adjNode *newNode;
	
	list = createAdjList(adj->V);
	
	for(v=0;v<adj->V;v++)
		for(w=v+1;w<adj->V;w++){
			if(adj->adjMat[v][w]==1){
				newNode = newAdjListNode(w);
				newNode->next = list[v].head;
				list[v].head = newNode;
	
				newNode = newAdjListNode(v);
				newNode->next = list[w].head;
				list[w].head = newNode;
			}
		}
	
	return list;
}

Graph* readGraph(char* filename, short int V){
	FILE* file;
	Graph* graph;
	short int src, dest, weight;
	
	file = fopen(filename,"r");
	
	if(file==NULL){
		printf("%s - no such file\n",filename);
		return NULL;
	}
	
	graph = createGraph(V,'l');

	fscanf(file,"%hd %hd %hd",&src,&dest,&weight);
	addEdge(graph,src-1,dest-1);
	
	while(!feof(file)){
		fscanf(file,"%hd %hd %hd\n",&src,&dest,&weight);
		addEdge(graph,src-1,dest-1);
	}

	fclose(file);
	return graph;
}

void printMatrix(const short int** matrix,short int N){
	short int i,j;
	
	if(N>20){
		printf("Matrix too large\n");
		return;
	}
	
	if(matrix == NULL){
		printf("Graph is NULL\n");
		exit(0);
	}
	
	for(i=0;i<N;i++){
		for(j=0;j<N;j++)
			printf("%hd ",matrix[i][j]);
		printf("\n");
	}
}

void printAdjList(const adjList *list, short int V){
	short int v;
	adjNode* pCrawl;
	
	if(V>20){
		printf("Adjacency list too large\n");
		return;
	}
	if(list==NULL){
		printf("Graph is NULL\n");
		exit(0);
	}
	
	for(v=0;v < V;v++){
		pCrawl = list[v].head;
		printf("%hd (head) ",v+1);
		while(pCrawl!=NULL){
			printf("-> %hd",pCrawl->dest+1);
			pCrawl = pCrawl->next;
		}
		printf("\n");
	}
}

void freeMatrix(short int** mat, short int N){
	short int i;
	
	if(mat == NULL) return;
	
	for(i=0;i<N;i++)
		free(mat[i]);
	free(mat);
}

void freeAdjList(adjList* list, short int V){
	short int v;
	adjNode *pCrawl, *temp;
	
	if(list == NULL) return;
	
	for(v=0;v < V;v++){
		pCrawl = list[v].head;

		while(pCrawl!=NULL){
			temp = pCrawl;
			pCrawl = pCrawl->next;
			free(temp);
		}
	}
	
	free(list);
}

