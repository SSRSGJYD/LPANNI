#pragma once

#include <vector>
#include <set>
using namespace std;

typedef struct ToEdge {
	ToEdge(int id):id(id) {}
	int id;
	float sim;
	float nni;
	bool operator==(const ToEdge b) const
	{
		return this->id == b.id;
	}
	bool operator!=(const ToEdge b) const
	{
		return this->id != b.id;
	}
	bool operator<=(const ToEdge b) const
	{
		return this->id <= b.id;
	}
	bool operator<(const ToEdge b) const
	{
		return this->id < b.id;
	}
	bool operator>=(const ToEdge b) const
	{
		return this->id >= b.id;
	}
	bool operator>(const ToEdge b) const
	{
		return this->id > b.id;
	}
} ToEdge;

typedef struct Node {
	Node(): id(0), e(0) {}
	unsigned int id;
	unsigned int e; // number of triangles
	float NI;
	set<ToEdge> adjNodes;
} Node;

class Graph 
{
public:
	Graph();
	Graph(string file);
	unsigned int nodeNum, edgeNum;
	vector<Node> nodes; // starts from nodes[1]
	void printNI();
};
