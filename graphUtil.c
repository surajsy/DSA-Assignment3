#include <stdio.h>
#include <stdlib.h>
#include "graphUtil.h"
#include "queue.h"

int* createIntArray(int V){
	int* array;
	int i;
	array = ALLOC_N(int,V);
	for(i=0;i<V;i++)
		array[i]=0;
	return array;
}

float* createFloatArray(int V){
	float* array;
	int i;
	array = ALLOC_N(float,V);
	for(i=0;i<V;i++)
		array[i]=0;
	return array;
}

int* createMatrix(int V){
	int i,v;
	int* adjMatrix;
	
	adjMatrix = ALLOC_N(int,V*(V+1)/2);
	
	for(v=0;v<V;v++)
		for(i=0;i<=v;i++){
//			printf("%d %d\n",v,i);
			adjMatrix[ind(v,i,V)] = 0;
		}
	return adjMatrix;
}

/*int ind(int i,int j,int V){
	if(i>=V || j>=V){
		printf("Invalid matrix index!\n");
		exit(0);
	}
		
	if(i>=j)
		return (int)(i*(i+1)/2 + j);
	else
		return (int)(j*(j+1)/2 + i);
}*/

adjNode* newAdjListNode(int dest){
	adjNode* newNode;
	newNode = ALLOC(adjNode);
	newNode->dest = dest;
	newNode->next = NULL;
	return newNode;
}

adjList* createAdjList(int V){
	adjList* adjLists;
	int i;
	
	adjLists = ALLOC_N(adjList,V);
	for(i=0;i<V;i++){
		adjLists[i].head = NULL;
	}
	
	return adjLists;
}

Graph* createGraph(int V, char format){
	int i;
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

/*int** initPathMatrix(int** adj, int V){
	int v,w;
	int** mat;
	
	mat = createMatrix(V);
	
	for(v=0;v<adj->V;v++)
		for(w=0;w<V;w++)
			if(adj[v][w]==1)
				mat[v][w]=w+1;
			else if (v==w)
				mat[v][w]=v+1;
	
	return mat;
}*/

int* initDistMatrix(int* adj, int V){
	int v,w;
	int* mat;
	
	mat = createMatrix(V);
	
	for(v=0;v<V;v++)
		for(w=0;w<v;w++)
			if(adj[ind(v,w,V)]==1)
				mat[ind(v,w,V)]=1;
			else
				mat[ind(v,w,V)]=INFINITY;
	
	return mat;
}

int* copyMatrix(int* adj, int V){
	int v,w;
	int* mat;
	
	mat = createMatrix(V);
	
	for(v=0;v<V;v++)
		for(w=0;w<v;w++)
			mat[ind(v,w,V)]=adj[ind(v,w,V)];
			
	return mat;
}

void addEdge(Graph* graph, int src, int dest){
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

int* ccBfs(Graph *graph){
	int v,v0,count;
	adjNode *pCrawl;
	Queue* q;
	int *component;
	
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

int findMaxInd(int* array,int N){
	int i,max,index;
	
	max=-1;
	for(i=0;i<N;i++){
		if(array[i]>max){
			max = array[i];
			index = i;
		}
	}
	return index;
}

int* largestComp(int* array,int N){
	int nComp,i;
	int* compSize;
	int* comp;
	
	nComp = array[findMaxInd(array,N)];
	compSize = ALLOC_N(int,nComp);
	
	for(i=0;i<nComp;i++)
		compSize[i] = 0;
		
	for(i=0;i<N;i++){
		compSize[array[i]-1]++;
	}

	comp = ALLOC_N(int,2);
	comp[0] = findMaxInd(compSize,nComp)+1;
	comp[1] = compSize[comp[0]-1];
	
	free(compSize);
	return comp;
}

Graph* extractComp(Graph* graph,int *component, int compN,int compSize){
	int *adjMatGraph;
	Graph *adjMatComp;
	int v,w,Vg,Vc;
	int m,n;
	FILE *f;

	Vg = graph->V;
	Vc = compSize;
	adjMatGraph = adjList2Mat(graph);
	
	adjMatComp = createGraph(Vc,'m');
	
	printf("Finding largest component and writing it to component.txt\n");
	if(Vc==Vg){
		adjMatComp->adjMat = adjMatGraph;
		f =  fopen("component.txt","w");
		for(v=0;v<Vg;v++)
			for(w=0;w<v;w++)
				fprintf(f,"%d %d 1\n", w+1, v+1);
		
		fclose(f);
		return adjMatComp;
	}
	
	adjMatComp->adjMat = createMatrix(Vc);

	m=-1;
	n=0;
//	printf("%d\n",Vc);
	f =  fopen("component.txt","w");
	for(v=0;v<Vg;v++){
		if(component[v] == compN){
			m++;
			n=0;
			
			for(w=0;w<v;w++){
				if(component[w]==compN){
//					printf("%d %d\n",m,n );
					adjMatComp->adjMat[ind(m,n,Vc)]=adjMatGraph[ind(v,w,Vg)];
					fprintf(f,"%d %d 1\n", n+1, m+1);
					n++;
				}
			}
		}
	}
	fclose(f);

	freeMatrix(adjMatGraph,Vg);

	return adjMatComp;
}

int* adjList2Mat(Graph* graph){
	int* adjMatrix;
	int v,i;
	adjNode* pCrawl;
	
	adjMatrix = createMatrix(graph->V);
	
	for(v=0;v<graph->V;v++){
		pCrawl = graph->adjLists[v].head;
		while(pCrawl!=NULL){
			adjMatrix[ind(v,pCrawl->dest,graph->V)]=1;
			pCrawl = pCrawl->next;
		}
	}
	
	return adjMatrix;
}

adjList* mat2AdjList(Graph* adj){
	adjList* list;
	int v,w;
	adjNode *newNode;
	
	list = createAdjList(adj->V);
	
	for(v=0;v<adj->V;v++)
		for(w=0;w<v;w++){
			if(adj->adjMat[ind(v,w,adj->V)]==1){
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

Graph* readGraph(char* filename, int V){
	FILE* file;
	Graph* graph;
	int src, dest, weight;
	
	file = fopen(filename,"r");
	
	if(file==NULL){
		printf("%s - no such file\n",filename);
		return NULL;
	}
	
	graph = createGraph(V,'l');

	fscanf(file,"%d %d %d",&src,&dest,&weight);
	addEdge(graph,src-1,dest-1);
	
	while(!feof(file)){
		fscanf(file,"%d %d %d\n",&src,&dest,&weight);
		addEdge(graph,src-1,dest-1);
	}

	fclose(file);
	return graph;
}

void printMatrix(const int* matrix,int N){
	int i,j;
	
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
			printf("%5hd ",matrix[ind(i,j,N)]);
		printf("\n");
	}
}

void printAdjList(const adjList *list, int V){
	int v;
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
		printf("%d (head) ",v+1);
		while(pCrawl!=NULL){
			printf("-> %d",pCrawl->dest+1);
			pCrawl = pCrawl->next;
		}
		printf("\n");
	}
}

void freeMatrix(int* mat, int N){
	int i;
	
	if(mat == NULL) return;
	free(mat);
}

void freeAdjList(adjList* list, int V){
	int v;
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

