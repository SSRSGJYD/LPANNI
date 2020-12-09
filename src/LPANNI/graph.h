#pragma once

#include <set>
#include <vector>
using namespace std;

typedef struct ToEdgeInfo {
	ToEdgeInfo() : s(0) {}
	~ToEdgeInfo(){}
	float s;
	float sim;
	float nni; // nni of neighbor node to this node
} ToEdgeInfo;

class Graph;
ToEdgeInfo* getEdgeInfo(Graph& g);

typedef struct ToEdge {
	ToEdge(int id, Graph& g, bool info=false) :id(id) { 
		if (info)
			pInfo = getEdgeInfo(g);
		else
			pInfo = nullptr;
	}
	int id;
	ToEdgeInfo *pInfo;
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
	float total_s;
	set<ToEdge> adjNodes;
	vector<unsigned int> communities;
	unsigned int label_set_size;
	unsigned int dominant_c;
	float dominant_b;
} Node;

typedef struct vQueueElem {
	vQueueElem(unsigned int id, float NI) : id(id), NI(NI) {}
	unsigned int id;
	float NI;
	bool operator==(const vQueueElem b) const
	{
		return this->NI == b.NI;
	}
	bool operator!=(const vQueueElem b) const
	{
		return this->NI != b.NI;
	}
	bool operator<=(const vQueueElem b) const
	{
		return this->NI <= b.NI;
	}
	bool operator<(const vQueueElem b) const
	{
		return this->NI < b.NI;
	}
	bool operator>=(const vQueueElem b) const
	{
		return this->NI >= b.NI;
	}
	bool operator>(const vQueueElem b) const
	{
		return this->NI > b.NI;
	}
} vQueueElem;

class Graph 
{
public:
	Graph();
	Graph(string file);
	~Graph();

	void printNI();
	void printSim();
	void printNNI();
	void printCommunityPerNode();
	void saveCommunity(string file);

public:
	unsigned int nodeNum, edgeNum;
	vector<Node> nodes; // starts from nodes[1]
	ToEdgeInfo *EdgeInfoPool;
	unsigned int firstValidPool;
};
