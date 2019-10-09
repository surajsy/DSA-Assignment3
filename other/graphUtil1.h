#ifndef GRAPHUTIL_H
#define GRAPHUTIL_H

#define ALLOC(x) (x*)malloc(sizeof(x))
#define ALLOC_N(x,n) (x*)malloc(n*sizeof(x))

#define NUM 0
#define SIZE 1
#define INFINITY 32767

struct adjnode{
	short int dest;
	struct adjnode* next;
};

struct adjlist{
	struct adjnode *head;
};

struct graph{
	short int V;
	struct adjlist* adjLists;
	short int **adjMat;
}; 

typedef struct adjnode adjNode;
typedef struct adjlist adjList;

#ifndef GRAPHSTRUCT
#define GRAPHSTRUCT
typedef struct graph Graph;
#endif

short int* createIntArray(short int V);
float* createFloatArray(short int V);
short int** createMatrix(short int V);
adjNode* newAdjListNode(short int dest);
adjList* createAdjList(short int V);
Graph* createGraph(short int V, char format);

//short int** initPathMatrix(short int** adj, short int V);
short int** initDistMatrix(short int** adj, short int V);
short int** copyMatrix(short int** adj, short int V);

void addEdge(Graph* graph, short int src, short int dest);
//adjNode* copyAdjList(adjNode* head);

short int* ccBfs(Graph *graph);
short int findMaxInd(short int* array,short int N);
short int* largestComp(short int* array,short int N);
Graph* extractComp(Graph* g,short int* component, short int compN,short int compSize);

short int** adjList2Mat(Graph* graph);
adjList* mat2AdjList(Graph* graph);

Graph* readGraph(char* filename, short int V);

void printMatrix(const short int** matrix,short int N);
void printAdjList(const adjList *list, short int V);

void freeMatrix(short int** mat, short int N);
void freeAdjList(adjList* list, short int V);

#endif