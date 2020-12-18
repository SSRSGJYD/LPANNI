import json
import os
from cdlib import NodeClustering, evaluation
import networkx as nx

def get_communities(path, first_delimiter, delimiter, seperate_comm_id):
    comm_dict = dict()
    with open(path, "rt") as f:
        for row in f:
            node, row = row.rstrip().split(first_delimiter, 1)
            node = int(node)
            record = list(map(int, row.split(delimiter)))
            for comm_id in record:
                if comm_id in comm_dict.keys():
                    comm_dict[comm_id].append(node)
                else:
                    comm_dict[comm_id] = [node]
    communities = []
    for k, v in comm_dict.items():
        if not seperate_comm_id:
            v.append(k)
        communities.append(v)
    return communities

if __name__ == "__main__":
    NMI_result = dict()
    Omega_result = dict()
    for N in [1000, 5000, 10000]:
        NMI_result[N] = dict()
        Omega_result[N] = dict()
        for mu in [0.1, 0.3]:
            NMI_result[N][mu] = {'NMI':[], 'NMI_max':[]}
            Omega_result[N][mu] = {'omega':[]}
            for om in range(2,9):
                name = 'LFR/N-{}-mu{:1.1f}-om{}'.format(N, mu, om)
                communities = get_communities(os.path.join("../../data/", name, "result.dat"), " ", " ", False)
                gt_communities = get_communities(os.path.join("../../data/", name, "community.dat"), "\t", " ", True)
                G = nx.read_adjlist(os.path.join("../../data/", name, "network.dat"), nodetype=int)
                coms = NodeClustering(communities, G, "", overlap=True)
                gt_coms = NodeClustering(gt_communities, G, "", overlap=True)
                nmi = evaluation.overlapping_normalized_mutual_information_LFK(coms, gt_coms)[0]
                nmi_max = evaluation.overlapping_normalized_mutual_information_MGH(coms, gt_coms)[0]
                omega = evaluation.omega(coms, gt_coms)[0]
                NMI_result[N][mu]['NMI'].append(nmi)
                NMI_result[N][mu]['NMI_max'].append(nmi_max)
                Omega_result[N][mu]['omega'].append(omega)

        with open('LFR_nmi.json', 'w') as f:
            json.dump(NMI_result, f)
        with open('LFR_omega.json', 'w') as f:
            json.dump(Omega_result, f)