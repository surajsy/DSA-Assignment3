#ifndef GRAPHUTIL_H
#define GRAPHUTIL_H

#define ALLOC(x) (x*)malloc(sizeof(x))
#define ALLOC_N(x,n) (x*)malloc(n*sizeof(x))
#define ind(i,j,V) (i>=j)?(i*(i+1)/2 + j):(j*(j+1)/2 + i)

#define NUM 0
#define SIZE 1
#define INFINITY 9999999

struct adjnode{
	int dest;
	struct adjnode* next;
};

struct adjlist{
	struct adjnode *head;
};

struct graph{
	int V;
	struct adjlist* adjLists;
	int *adjMat;
}; 

typedef struct adjnode adjNode;
typedef struct adjlist adjList;

#ifndef GRAPHSTRUCT
#define GRAPHSTRUCT
typedef struct graph Graph;
#endif

int* createIntArray(int V);
float* createFloatArray(int V);
int* createMatrix(int V);
//int ind(int i,int j,int V);
adjNode* newAdjListNode(int dest);
adjList* createAdjList(int V);
Graph* createGraph(int V, char format);

//int** initPathMatrix(int** adj, int V);
int* initDistMatrix(int* adj, int V);
int* copyMatrix(int* adj, int V);

void addEdge(Graph* graph, int src, int dest);
//adjNode* copyAdjList(adjNode* head);

int* ccBfs(Graph *graph);
int findMaxInd(int* array,int N);
int* largestComp(int* array,int N);
Graph* extractComp(Graph* g,int* component, int compN,int compSize);

int* adjList2Mat(Graph* graph);
adjList* mat2AdjList(Graph* graph);

Graph* readGraph(char* filename, int V);

void printMatrix(const int* matrix,int N);
void printAdjList(const adjList *list, int V);

void freeMatrix(int* mat, int N);
void freeAdjList(adjList* list, int V);

#endif