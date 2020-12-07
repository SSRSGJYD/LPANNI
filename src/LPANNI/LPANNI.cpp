// LPANNI.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "algorithm.h"
#include "graph.h"

int main()
{
	//Graph graph = Graph(string("../../data/Amazon/amazon.ungraph.txt"));
	Graph graph = Graph(string("../../data/sample.txt"));
	
	LPANNI worker = LPANNI();
	worker.calculate_NI(graph);
	graph.printNI();

	worker.calculate_SIM_NNI(graph, 3);
	graph.printSim();
	graph.printNNI();

	worker.propagation(graph, 3);
	graph.printCommunityPerNode();

	return 0;
}