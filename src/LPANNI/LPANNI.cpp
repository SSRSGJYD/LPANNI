// LPANNI.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

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
	float total_time = 0;

	//for (unsigned int i = 0; i < repeat; i++) {
	//	//Graph graph = Graph(string("../../data/sample.txt"));
	//	Graph graph = Graph(string("../../data/amazon/amazon.ungraph.txt"));
	//	
	//	clock_t start_time = clock();
	//	worker.calculate_NI(graph);
	//	// graph.printni();

	//	worker.calculate_SIM_NNI(graph, alpha);
	//	// graph.printsim();
	//	// graph.printnni();

	//	worker.propagation(graph, T);
	//	// graph.printcommunitypernode();

	//	clock_t end_time = clock();
	//	float t = (end_time - start_time) / (double)CLOCKS_PER_SEC;
	//	total_time += t;

	//	/*char save_name[50];
	//	sprintf_s(save_name, 50, "../../result/amazon_%d_%d.txt", alpha, t);
	//	file *fp;
	//	fopen_s(&fp, save_name, "w");
	//	char time[50];
	//	unsigned int offset = sprintf_s(time, 50, "%.3f\n", t);
	//	fwrite(time, offset, 1, fp);
	//	fclose(fp);
	//	graph.savecommunity(string(save_name));*/

	//	cout << "time: " << t << endl;
	//}
	//return 0;

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
	vector<unsigned int> Ns = { 1000,2000,3000,4000,5000,6000,7000,8000,9000,10000 };
	for (unsigned int N : Ns) {
		float total_time = 0;
		for (unsigned int i = 0; i < repeat; i++) {
			char save_name[50];
			sprintf_s(save_name, 50, "../../time complexity/LFR/N-%d-mu0.1-om3", N);
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
	}
	
	return 0;
}