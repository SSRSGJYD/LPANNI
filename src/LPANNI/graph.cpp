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
	for (unsigned i = 0; i < this->nodeNum; i++) {
		this->nodes[i].id = i + 1;
	}
	// read in edges
	sgetline(p);
	sgetline(p);
	unsigned int s, e;
	while (**p != '\0') {
		s = sgetunsigned(p);
		sgetstr(p);
		e = sgetunsigned(p);
		this->nodes[s - 1].adjNodes.insert(e);
		this->nodes[e - 1].adjNodes.insert(s);
		if (**p == '\0')
			break;
		sgetstr(p);
	}
	free(text);
	return;
}
