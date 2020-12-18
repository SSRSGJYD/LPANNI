#include <iostream>
#include <sstream>
#include <time.h> 
#include "algorithm.h"
#include "graph.h"

int main()
{
	LPANNI worker = LPANNI();
	unsigned int alpha = 3;
	unsigned int T = 100;
	unsigned int repeat = 10;

	// Amazon
	Graph graph = Graph(string("../../data/amazon/amazon.ungraph.txt"));
	clock_t start_time = clock();
	worker.calculate_NI(graph);
	// graph.printni();
	worker.calculate_SIM_NNI(graph, alpha);
	// graph.printsim();
	// graph.printnni();
	worker.propagation(graph, T);
	// graph.printcommunitypernode();
	char save_name[50];
	sprintf_s(save_name, 50, "../../result/amazon_%d_%d.txt", alpha, t);
	graph.saveCommunity(string(save_name));

    // LFR benchmark
	/*vector<unsigned int> Ns = { 1000,5000,10000 };
	vector<float> mus = { 0.1, 0.3 };
	vector<unsigned int> oms = { 2,3,4,5,6,7,8 };
	for (unsigned int N : Ns) {
		for (float mu : mus) {
			for (unsigned om : oms) {
				char save_name[50];
				sprintf_s(save_name, 50, "../../data/LFR/N-%d-mu%.1f-om%d", N, mu, om);
				Graph graph = Graph(string(save_name) + string("/network.dat"));
				worker.calculate_NI(graph);
				worker.calculate_SIM_NNI(graph, alpha);
				worker.propagation(graph, T);
				graph.saveCommunity(string(save_name) + string("/result.dat"));
			}
		}
	}*/

	// LRF time complexity
	/*vector<unsigned int> Ns = { 10000,20000,30000,40000,50000,60000,70000,80000,90000,100000 };
	for (unsigned int N : Ns) {
		float total_time = 0;
		for (unsigned int i = 0; i < repeat; i++) {
			char save_name[50];
			sprintf_s(save_name, 50, "../../data/time complexity/N-%d-mu0.1-om3", N);
			
			Graph graph = Graph(string(save_name) + string("/network.dat"));
			
			clock_t start_time = clock();
			worker.calculate_NI(graph);
			worker.calculate_SIM_NNI(graph, alpha);
			worker.propagation(graph, T);
			clock_t end_time = clock();
			float t = (end_time - start_time) / (double)CLOCKS_PER_SEC;
			total_time += t;
		}
		cout << "N=" << N << ", time: " << total_time / repeat << endl;
	}*/
	
	return 0;
}