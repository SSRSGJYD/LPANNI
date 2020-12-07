#include <stdio.h>
#include "graph.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

Graph::Graph()
{
}

void sgetline(char **text) {
	while (**text != '\n') {
		(*text)++;
	}
	(*text)++;
}

void sgetstr(char **text) {
	while (**text > '9' || **text < '0') {
		(*text)++;
	}
}

unsigned sgetunsigned(char **text) {
	unsigned num = 0;
	while (**text <= '9' && **text >= '0') {
		num *= 10;
		num += **text - '0';
		(*text)++;
	}
	return num;
}

Graph::Graph(string file)
{
	FILE* fp;
	fopen_s(&fp, file.c_str(), "r");
	if (fp == NULL) {
		return;
	}
	// build graph from Amazon dataset
	fseek(fp, 0, SEEK_END);
	long lSize = ftell(fp);
	char *text = (char*)malloc(lSize + 1);
	rewind(fp);
	fread(text, sizeof(char), lSize, fp);
	text[lSize] = '\0';

	char *str = text;
	char **p = &str;
	sgetline(p);
	sgetline(p);
	sgetstr(p);
	this->nodeNum = sgetunsigned(p);
	sgetstr(p);
	this->edgeNum = sgetunsigned(p);
	this->nodes.resize(550000);
	this->EdgeInfoPool = new ToEdgeInfo[2 * this->edgeNum + 1];
	this->firstValidPool = 0;

	// read in edges
	sgetline(p);
	sgetline(p);
	unsigned int s, e;
	while (**p != '\0') {
		s = sgetunsigned(p);
		sgetstr(p);
		e = sgetunsigned(p);
		
		this->nodes[s].adjNodes.insert(ToEdge(e, *this, true));
		this->nodes[e].adjNodes.insert(ToEdge(s, *this, true));
		this->nodes[s].id = s;
		this->nodes[e].id = e;
		if (**p == '\0')
			break;
		sgetstr(p);
	}
	free(text);
	return;
}

Graph::~Graph()
{
	delete [] this->EdgeInfoPool;
}

void Graph::printNI()
{
	auto endit = this->nodes.end();
	for (auto it = this->nodes.begin(); it != endit; it++) {
		if (it->id == 0) continue;
		cout << "id: " << it->id << ", NI: " << it->NI << endl;
	}
	return;
}

void Graph::printSim()
{
	auto endit = this->nodes.end();
	for (auto it = this->nodes.begin(); it != endit; it++) {
		if (it->id == 0) continue;
		cout << "id: " << it->id << ", Sim: ";
		auto sendit = it->adjNodes.end();
		for (auto sit = it->adjNodes.begin(); sit != sendit; sit++) {
			cout << sit->pInfo->sim << " ";
		}
		cout << endl;
	}
	return;
}

void Graph::printNNI()
{
	auto endit = this->nodes.end();
	for (auto it = this->nodes.begin(); it != endit; it++) {
		if (it->id == 0) continue;
		cout << "id: " << it->id << ", NNI: ";
		auto sendit = it->adjNodes.end();
		for (auto sit = it->adjNodes.begin(); sit != sendit; sit++) {
			cout << sit->pInfo->nni << " ";
		}
		cout << endl;
	}
	return;
}

ToEdgeInfo * getEdgeInfo(Graph & g)
{
	g.firstValidPool += 1;
	return g.EdgeInfoPool + (g.firstValidPool - 1);
}
