#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"

int main(){
	int Vg,Vc,i;
	float prob;
	char *graphFile,*cenFile1,*cenFile2;
	Centrality *c;
	Graph *graph;
	FILE* f;
	clock_t start,end;
	double cpuTime;

/*	cenFile1 = "fwBCDefault1.txt";
	cenFile2 = "bfsBCDefault2.txt";
	graphFile = "default.txt";
	Vg = 327;*/
	cenFile1 = "fwBCTest1.txt";
	cenFile2 = "bfsBCTest2.txt";
	graphFile = "testGraph.txt";
	
	printf("Enter parameters: V and p\n");
	scanf("%d %f",&Vg,&prob);
	randGraphGen(Vg,prob,graphFile);
	
	graph = initialize(Vg,graphFile);
//	printf("Largest Component\n");
//	printGraph(graph);
	
	start = clock();
	c = fwBCentrality(graph);
	end = clock();

	cpuTime = (double)(end - start) / CLOCKS_PER_SEC;
	printf("CPU time for BC using FW algo: %f s\n",cpuTime);

	f = fopen(cenFile1,"w");
	for(i=0;i<c->V;i++)
		fprintf(f,"%d\t%.5f\n",i+1,c->BCVal[i]);
	fclose(f);

	free(c->BCVal);
	free(c);
	
	switchRepr(graph,'l');
//	printGraph(graph);
	
	start = clock();
	c = bfsBCentrality(graph);
	end = clock();

	cpuTime = (double)(end - start) / CLOCKS_PER_SEC;
	printf("CPU time for BC using BFS algo: %f s\n",cpuTime);
	
	f = fopen(cenFile2,"w");
	for(i=0;i<c->V;i++)
		fprintf(f,"%d\t%.5f\n",i+1,c->BCVal[i]);
	fclose(f);
	
	free(c->BCVal);
	free(c);
	
	deleteGraph(graph);
}