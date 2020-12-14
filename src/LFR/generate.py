import os
import re
import shutil

if __name__ == "__main__":
    dirname = 'LFR'
    for N,k,maxk,minc,maxc,on in [(1000,10,50,10,50,100), (5000,10,50,20,100,500), (10000,20,100,20,100,2000)]:
        for mu in [0.1, 0.3]:
            for om in range(2,9):
                if not os.path.exists('{}/N-{}-mu{:1.1f}-om{}'.format(dirname, N, mu, om)):
                    os.makedirs('{}/N-{}-mu{:1.1f}-om{}'.format(dirname, N, mu, om))
                s = os.popen('.\\benchmark -k {} -maxk {} -mu {} -minc {} -maxc {} -om {} -N {} -on {}'.format(k, maxk, mu, minc, maxc, om, N, on)).readlines()
                ans = re.match('network\sof\s(\d*)\svertices\sand\s(\d*)\sedges', s[-6])
                node_num = int(ans[1])
                edge_num = int(ans[2])
                with open('./network.dat', 'r') as f:
                    lines = f.readlines()
                with open('./network.dat', 'w', newline='\n') as f:
                    f.write('{}\n{}\n'.format(node_num, edge_num))
                    f.writelines(lines)
                shutil.move('./network.dat', '{}/N-{}-mu{:1.1f}-om{}/network.dat'.format(dirname, N, mu, om))
                shutil.move('./community.dat', '{}/N-{}-mu{:1.1f}-om{}/community.dat'.format(dirname, N, mu, om))
                shutil.move('./statistics.dat', '{}/N-{}-mu{:1.1f}-om{}/statistics.dat'.format(dirname, N, mu, om))
                shutil.move('./time_seed.dat', '{}/N-{}-mu{:1.1f}-om{}/time_seed.dat'.format(dirname, N, mu, om))