centrality.out: main.o randGraph.o graphUtil.o graphMain.o queue.o
	g++ -g main.o randGraph.o graphUtil.o graphMain.o queue.o -o centrality.out
	
main.o: main.c graph.h
	gcc -g -c main.c
	
randGraph.o: randGraph.cpp
	g++ -g -c randGraph.cpp
	
graphMain.o: graphMain.c graph.h graphUtil.h
	gcc -g -c graphMain.c

graphUtil.o: graphUtil.c graphUtil.h queue.h
	gcc -g -c graphUtil.c
	
queue.o: queue.c queue.h
	gcc -g -c queue.c