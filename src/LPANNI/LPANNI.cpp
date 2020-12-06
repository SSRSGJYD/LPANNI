// LPANNI.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "graph.h"

int main()
{
	Graph graph = Graph(string("../../data/Amazon/amazon.ungraph.txt"));
	//Graph graph = Graph(string("../../data/Amazon/amazon.ungraph_few.txt"));
	cout << graph.nodes.size();
	return 0;
}