#pragma once

#include "graph.h"

class LPANNI
{
public:
	LPANNI() {};
	void calculate_NI(Graph& g);
	void calculate_SIM_NNI(Graph& g, unsigned int alpha);
	void propagation(Graph& g, unsigned int T);
};