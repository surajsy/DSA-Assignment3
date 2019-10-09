#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <iostream>
#include <fstream>
#include <string>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
typedef boost::sorted_erdos_renyi_iterator<boost::mt19937, Graph> ERGen;
typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;
typedef boost::property_map<Graph, boost::vertex_index_t>::type IndexMap;

extern "C" void randGraphGen(int nodeNum, float edgeProb, char* filename);

void randGraphGen(int nodeNum, float edgeProb, char* filename){
	
	std::ofstream fileOut;
//	boost::minstd_rand gen;
	boost::mt19937 gen;
	
/*	nodeNum = std::__cxx11::stoi(argv[1]);
	edgeProb = std::__cxx11::stof(argv[2]);*/
	Graph g(ERGen(gen,nodeNum,edgeProb),ERGen(),nodeNum);
	
	IndexMap index = boost::get(boost::vertex_index,g);
	edge_iterator  ei, ei_end;
	std::tie (ei, ei_end) = edges(g);
	
	fileOut.open(filename);
	
	while(ei != ei_end){
		fileOut << index[source(*ei,g)] + 1 << ' ' << index[target(*ei,g)] + 1 << " 1" << std::endl;
		++ei;
	}
	
	fileOut.close();
	return;
}