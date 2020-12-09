#include <iostream>
#include <map>
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
		auto sendit = it->adjNodes.end();
		for (auto sit = it->adjNodes.upper_bound(ToEdge(it->id, g)); sit != sendit; sit++) {
			// work on this node and sit
			unsigned int plen = 1;
			vector<unsigned int> *pLastDest = nullptr;
			while (plen <= alpha) {
				vector<unsigned int> *pNewDest = new vector<unsigned int>();
				if (plen == 1) {
					for (auto sit2 = it->adjNodes.begin(); sit2 != sendit; sit2++) {
						if(sit2->id == sit->id)
							sit->pInfo->s += 1.0;
						else
							pNewDest->push_back(sit2->id);
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
							/*else
								pNewDest->push_back(sit3->id);*/
							else if(sit3->id != it->id)
								pNewDest->push_back(sit3->id);
						}
					}
				}
				delete pLastDest;
				pLastDest = pNewDest;
				plen++;
			}
			delete pLastDest;
			auto it2 = g.nodes[sit->id].adjNodes.find(ToEdge(it->id, g));
			it2->pInfo->s = sit->pInfo->s;
		}
		int a = 1;
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
		//float sum_sim = 0;
		for (auto sit = it->adjNodes.begin(); sit != sendit; sit++) {
			if (it->id == sit->id) continue;
			float sim = sit->pInfo->s / sqrt(it->total_s * g.nodes[sit->id].total_s);
			sit->pInfo->sim = sim;
			max_sim = sim > max_sim ? sim : max_sim;
			//sum_sim += sim;
		}

		// normalize sim to [0,1] and calculate NNI
		for (auto sit = it->adjNodes.begin(); sit != sendit; sit++) {
			if (it->id == sit->id) continue;
			sit->pInfo->nni = sqrt(g.nodes[sit->id].NI * sit->pInfo->sim / max_sim);
		}
	}

	return;
}

void LPANNI::propagation(Graph & g, unsigned int T)
{
	// sort the nodes by NI & initialize
	multiset<vQueueElem> vQueue;
	auto endit = g.nodes.end();
	for (auto it = g.nodes.begin(); it != endit; it++) {
		if (it->id == 0) continue;
		vQueue.insert(vQueueElem(it->id, it->NI));
		Node* pNode = &(g.nodes[it->id]);
		pNode->dominant_c = it->id;
		pNode->dominant_b = 1.0;
		pNode->label_set_size = 1;
	}

	// label propagation
	unsigned int t = 0;
	auto endsit = vQueue.end();
	while (t < T) {
		bool update = false;
		// for each node u in vQueue
		for (auto sit = vQueue.begin(); sit != endsit; sit++) {
			Node* pNode = &(g.nodes[sit->id]);
			set<ToEdge> *pAdjNodes = &(pNode->adjNodes);
			auto endsit2 = pAdjNodes->end();
			map<unsigned int, float> bMap; // <c, sum(b*NNI)>
			// find L_Ng and calculate b*NNI
			float total_bnni = 0;
			for (auto sit2 = pAdjNodes->begin(); sit2 != endsit2; sit2++) {
				Node* pNode = &(g.nodes[sit2->id]);
				unsigned c_id = pNode->dominant_c;
				float b = pNode->dominant_b;
				if (bMap.find(c_id) == bMap.end())
					// no c_id yet, add it into bMap now
					bMap[c_id] = b * sit2->pInfo->nni;
				else 
					bMap[c_id] += b * sit2->pInfo->nni;
				total_bnni += b * sit2->pInfo->nni;
			}
			// calculate b', filter by >= 1/|L'|
			float threshold = 1.0 / bMap.size();
			float total_b = 0;
			float max_b = 0;
			set<unsigned int> candidate_dominant;
			auto endmit = bMap.end();
			unsigned int label_set_size = 0;
			for (auto mit = bMap.begin(); mit != endmit; mit++) {
				float new_b = mit->second / total_bnni;
				if (new_b > threshold - 1e-6) {
					// find a valid <c,b>
					label_set_size += 1;
					total_b += new_b;
					if (new_b > max_b + 1e-6) {
						// update dominant label candidate
						candidate_dominant.clear();
						max_b = new_b;
						candidate_dominant.insert(mit->first);
					}
					else if (new_b > max_b - 1e-6) {
						// same b, also update dominant label candidate
						candidate_dominant.insert(mit->first);
					}
				}
			}
			if (candidate_dominant.count(pNode->dominant_c) == 0) {
				// update dominant label
				pNode->dominant_c = *(candidate_dominant.begin());
				update = true;
			}
			pNode->dominant_b = max_b / total_b;

			// judge whether size of label set changes
			if (label_set_size != pNode->label_set_size)
				update = true;

			// save L''
			pNode->communities.clear();
			for (auto mit = bMap.begin(); mit != endmit; mit++) {
				float new_b = mit->second / total_bnni;
				if (new_b > threshold - 1e-6) {
					pNode->communities.push_back(mit->first);
				}
			}
		}
		// finish one iteration
		if (!update) {
			break; // break if no update in this iteration
		}
			
		t += 1;
	}

	return;
}


