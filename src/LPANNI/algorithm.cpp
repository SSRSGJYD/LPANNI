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
		for (auto sit = it->adjNodes.upper_bound(ToEdge(it->id, g)); sit != sendit; sit++) {
			auto sit2 = sit;
			sit2++;
			while (sit2 != sendit) {
				if (g.nodes[sit->id].adjNodes.find(ToEdge(sit2->id, g)) != g.nodes[sit->id].adjNodes.end()) {
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

void LPANNI::calculate_SIM_NNI(Graph & g, unsigned int alpha)
{
	// calculate s(u,v)
	auto endit = g.nodes.end();
	for (auto it = g.nodes.begin(); it != endit; it++) {
		if (it->id == 0) continue;
		//cout << it->id << " ";
		auto sendit = it->adjNodes.end();
		for (auto sit = it->adjNodes.upper_bound(ToEdge(it->id, g)); sit != sendit; sit++) {
			// work on this node and sit
			//cout << sit->id << " ";
			unsigned int plen = 1;
			set<unsigned int> *pLastDest = nullptr;
			while (plen <= alpha) {
				set<unsigned int> *pNewDest = new set<unsigned int>();
				if (plen == 1) {
					for (auto sit2 = it->adjNodes.begin(); sit2 != sendit; sit2++) {
						if(sit2->id == sit->id)
							sit->pInfo->s += 1.0;
						else
							pNewDest->insert(sit2->id);
					}
				}
				else {
					auto endsit = pLastDest->end();
					for (auto sit2 = pLastDest->begin(); sit2 != endsit; sit2++) {
						// for each node with plen-1 path to the node
						auto endsit2 = g.nodes[*sit2].adjNodes.end();
						for (auto sit3 = g.nodes[*sit2].adjNodes.begin(); sit3 != endsit2; sit3++) {
							if(sit3->id == sit->id)
								sit->pInfo->s += 1.0 / plen;
							else
								pNewDest->insert(sit3->id);
						}
					}
				}
				free(pLastDest);
				pLastDest = pNewDest;
				plen++;
			}
			auto it2 = g.nodes[sit->id].adjNodes.find(ToEdge(it->id, g));
			it2->pInfo->s = sit->pInfo->s;
		}
	}

	// calculate sum of s for each node
	for (auto it = g.nodes.begin(); it != endit; it++) {
		if (it->id == 0) continue;
		float total_s = 0;
		auto sendit = it->adjNodes.end();
		for (auto sit = it->adjNodes.begin(); sit != sendit; sit++) {
			total_s += sit->pInfo->s;
		}
		it->total_s = total_s;
	}

	// calculate Sim
	for (auto it = g.nodes.begin(); it != endit; it++) {
		if (it->id == 0) continue;
		auto sendit = it->adjNodes.end();
		// calculate sim of (u,v)
		float max_sim = 0;
		for (auto sit = it->adjNodes.begin(); sit != sendit; sit++) {
			if (it->id == sit->id) continue;
			float sim = sit->pInfo->s / sqrt(it->total_s * g.nodes[sit->id].total_s);
			sit->pInfo->sim = sim;
			max_sim = sim > max_sim ? sim : max_sim;
		}

		// normalize sim to [0,1] and calculate NNI
		for (auto sit = it->adjNodes.begin(); sit != sendit; sit++) {
			if (it->id == sit->id) continue;
			sit->pInfo->nni = sqrt(g.nodes[sit->id].NI * sit->pInfo->sim / max_sim);
		}
	}

	return;
}


