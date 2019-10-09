ReadMe for Assignment 3: Betweenness Centrality using Floyd Warshall and BFS

Header file: graph.h

Functions

randGraphGen	Takes no. of vertices, probability of edge and file name as input and outputs a random graph into the file

initialize	Finds the largest component of the graph in the given file and initializes a graph structure with the component

switchRepr	Changes graph representation from adjacency list to adjacency matrix and vice versa

fwBCentrality	Runs Floyd Warshall APSP algorithm and computes betweenness centrality

bfsBCentrality	Runs BFS to find APSP and computes betweenness centrality

printGraph	Prints the graph in the format it is currently in (Adjacency Matrix/Adjacency List)

deleteGraph	Deletes the graph structure
