#include <iostream>
#include "algorithm.h"

void LPANNI::calculate_NI(Graph & g)
{
	int max_e_plus_k = INT_MIN;
	int min_e_plus_k = INT_MAX;
	auto endit = g.nodes.end();
	for (auto it = g.nodes.begin(); it != endit; it++) {
		if (it->id == 0) continue;
		// find triangles
		auto sendit = it->adjNodes.end();
		for (auto sit = it->adjNodes.upper_bound(it->id); sit != sendit; sit++) {
			auto sit2 = sit;
			sit2++;
			while (sit2 != sendit) {
				if (g.nodes[sit->id].adjNodes.find(sit2->id) != g.nodes[sit->id].adjNodes.end()) {
					// find a triangle
					it->e += 1;
					g.nodes[sit->id].e += 1;
					g.nodes[sit2->id].e += 1;
				}
				sit2++;
			}
		}
		// update min(e+k) and min(e-k)
		int e_plus_k = it->e + (int)it->adjNodes.size();
		min_e_plus_k = e_plus_k < min_e_plus_k ? e_plus_k : min_e_plus_k;
		max_e_plus_k = e_plus_k > max_e_plus_k ? e_plus_k : max_e_plus_k;
	}
	std::cout << "max_e_plus_k: " << max_e_plus_k << ", min_e_plus_k: " << min_e_plus_k << std::endl;
	
	// calculate NI
	float diff = float(max_e_plus_k - min_e_plus_k);
	for (auto it = g.nodes.begin(); it != endit; it++) {
		if (it->id == 0) continue;
		float e_plus_k = float(it->e + it->adjNodes.size());
		it->NI = 0.5 * (1.0 + (e_plus_k - min_e_plus_k) / diff);
	}
	return;
}

void LPANNI::calculate_SIM(Graph & g)
{

}
