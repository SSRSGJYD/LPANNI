from cdlib import NodeClustering
from cdlib import evaluation
import networkx as nx


def get_communities(path, delimiter):
    comm_dict = dict()
    with open(path, "rt") as f:
        for row in f:
            node, row = row.rstrip().split(delimiter, 1)
            node = int(node)
            record = list(map(int, row.split(" ")))
            for comm_id in record:
                if comm_id in comm_dict.keys():
                    comm_dict[comm_id].append(node)
                else:
                    comm_dict[comm_id] = [node]
    communities = []
    for k, v in comm_dict.items():
        v.append(k)
        communities.append(v)
    print(len(communities))
    return communities


if __name__ == "__main__":
    f = open("nmi.txt", "w")
    for i in [1000, 5000, 10000]:
        for j in [1, 3]:
            for k in [2,3,4,5,6,7,8]:
                res_file_name = "../data/LFR/N-"+str(i)+"-mu0."+str(j)+"-om"+str(k)+"/result.dat"
                com_file_name = "../data/LFR/N-"+str(i)+"-mu0."+str(j)+"-om"+str(k)+"/community.dat"
                net_file_name = "../data/LFR/N-"+str(i)+"-mu0."+str(j)+"-om"+str(k)+"/network.dat"
                file_name = "N-"+str(i)+"-mu0."+str(j)+"-om"+str(k)
                communities = get_communities(res_file_name, " ")
                gt_communities = get_communities(com_file_name, "\t")
                G = nx.read_adjlist(net_file_name, nodetype=int)
                coms = NodeClustering(communities, G, "", overlap=True)
                gt_coms = NodeClustering(gt_communities, G, "", overlap=True)
                nmi = evaluation.overlapping_normalized_mutual_information_MGH(coms, gt_coms)
                f.write(file_name+" "+str(nmi[0])+'\n')
    f.close()
