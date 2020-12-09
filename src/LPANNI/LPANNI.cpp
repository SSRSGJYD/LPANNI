// LPANNI.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <time.h> 
#include "algorithm.h"
#include "graph.h"

int main()
{
	//Graph graph = Graph(string("../../data/Amazon/amazon.ungraph.txt"));
	Graph graph = Graph(string("../../data/sample.txt"));
	
	LPANNI worker = LPANNI();
	unsigned int alpha = 3;
	unsigned int T = 3;

	clock_t start_time = clock();
	worker.calculate_NI(graph);
	// graph.printNI();

	worker.calculate_SIM_NNI(graph, alpha);
	// graph.printSim();
	// graph.printNNI();

	worker.propagation(graph, T);
	// graph.printCommunityPerNode();

	clock_t end_time = clock();

	char save_name[50];
	sprintf_s(save_name, 50, "../../result/sample_%d_%d.txt", alpha, T);
	FILE *fp;
	fopen_s(&fp, save_name, "w");
	char time[50];
	unsigned int offset = sprintf_s(time, 50, "%.3f\n", (end_time - start_time)/(double)CLOCKS_PER_SEC);
	fwrite(time, offset, 1, fp);
	fclose(fp);
	graph.saveCommunity(string(save_name));

	return 0;
}