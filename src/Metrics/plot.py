import json
import matplotlib.pyplot as plt
import numpy as np

def plot_metric(result, key):
    fig = plt.figure(figsize=(8.5, 6.5))
    for i, N in enumerate([1000, 5000, 10000]):
        for j, mu in enumerate([0.1, 0.3]):
            ax1 = plt.subplot(2, 3, i+3*j+1)
            p1, = ax1.plot(list(range(2,9)), result[str(N)][str(mu)][key], marker='s')
            ax1.set_xlabel('om')
            ax1.set_ylabel(key)
            
            plt.xticks(range(2,9))
            plt.yticks(np.arange(0, 1.1, 0.1))
            plt.title('N-{}-mu{:1.1f}'.format(N, mu))

    plt.tight_layout()
    plt.savefig(fname='{}.png'.format(key), format='png', dpi=300)
    plt.cla()

if __name__ == "__main__":
    with open('LFR_nmi.json', 'r') as f:
        NMI_result = json.load(f)
    for key in ['NMI', 'NMI_max']:
        plot_metric(NMI_result, key)

    with open('LFR_omega.json', 'r') as f:
        Omega_result = json.load(f)
    plot_metric(Omega_result, 'omega')

    with open('FScore.json', 'r') as f:
        FScore_result = json.load(f)
    for key in ['Precision', 'Recall', 'FScore']:
        plot_metric(FScore_result, key)