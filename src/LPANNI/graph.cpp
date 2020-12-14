#include <stdio.h>
#include "graph.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

Graph::Graph()
{
}

void sgetline(char **text) {
	while (**text != '\n' && **text != '\r') {
		(*text)++;
	}
	(*text)++;
}

void sgetstr(char **text) {
	while (**text > '9' || **text < '0') {
		(*text)++;
		if (**text == '\0')
			break;
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
	this->nodeNum = sgetunsigned(p);
	sgetstr(p);
	this->edgeNum = sgetunsigned(p);
	sgetstr(p);
	//this->nodes.resize(550000);
	this->nodes.resize(this->nodeNum + 1);
	this->EdgeInfoPool = new ToEdgeInfo[2 * this->edgeNum + 1]();
	this->firstValidPool = 0;

	// read in edges
	unsigned int s, e;
	unsigned int count = 0;
	while (**p != '\0') {
		s = sgetunsigned(p);
		sgetstr(p);
		e = sgetunsigned(p);
		sgetstr(p);
		if (s < e) {
			this->nodes[s].adjNodes.insert(ToEdge(e, *this, true));
			this->nodes[e].adjNodes.insert(ToEdge(s, *this, true));
			this->nodes[s].id = s;
			this->nodes[e].id = e;	
		}
		if (**p == '\0')
			break;
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

void Graph::printCommunityPerNode()
{
	auto endit = this->nodes.end();
	for (auto it = this->nodes.begin(); it != endit; it++) {
		if (it->id == 0) continue;
		cout << "id: " << it->id << ", community: ";
		auto sendit = it->communities.end();
		for (auto sit = it->communities.begin(); sit != sendit; sit++) {
			cout << *sit << " ";
		}
		cout << endl;
	}
	return;
}

void Graph::saveCommunity(string file)
{
	FILE *fp;
	fopen_s(&fp, file.c_str(), "a");
	char *buffer = (char*)malloc(this->nodeNum + 10);
	unsigned int offset = 0;
	auto endit = this->nodes.end();
	for (auto it = this->nodes.begin(); it != endit; it++) {
		if (it->id == 0) continue;
		offset += sprintf_s(buffer + offset, this->nodeNum + 1 - offset, "%d", it->id);
		auto sendit = it->communities.end();
		for (auto sit = it->communities.begin(); sit != sendit; sit++) {
			offset += sprintf_s(buffer + offset, this->nodeNum + 1 - offset, " %d", *sit);
		}
		offset += sprintf_s(buffer + offset, this->nodeNum + 1 - offset, "\n");
		// when buffer is nearly full, write into file
		if (this->nodeNum + 1 < offset + 100) {
			fwrite(buffer, offset, 1, fp);
			offset = 0;
		}
	}
	if(offset > 0)
		fwrite(buffer, offset, 1, fp); // write remaining text
	fclose(fp);
	free(buffer);
	return;
}

ToEdgeInfo * getEdgeInfo(Graph & g)
{
	g.firstValidPool += 1;
	return g.EdgeInfoPool + (g.firstValidPool - 1);
}
