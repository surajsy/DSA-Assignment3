#ifndef GRAPH_H
#define GRAPH_H

struct BC{
	float *BCVal;
	int V;
};

typedef struct BC Centrality;

#ifndef GRAPHSTRUCT
#define GRAPHSTRUCT
typedef struct graph Graph;
#endif

void randGraphGen(int nodeNum, float edgeProb, char* filename);
Graph* initialize(int V, char* filename);

void switchRepr(Graph* graph, char format);

Centrality* fwBCentrality(Graph* adj);
Centrality* bfsBCentrality(Graph *graph);

void printGraph(const Graph* graph);
void deleteGraph(Graph* graph);

#endif 