#pragma once

#include <vector>
#include <set>
using namespace std;

typedef struct Node {
	unsigned int id;
	set<unsigned int> adjNodes;
} Node;

class Graph 
{
public:
	Graph();
	Graph(string file);
	unsigned int nodeNum, edgeNum;
	vector<Node> nodes;
};
