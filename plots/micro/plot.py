import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd
import numpy as np

plt.rcParams["font.size"] = 14
plt.rcParams["font.family"] = "Times New Roman"
plt.rcParams["pdf.fonttype"] = 42
plt.rcParams["ps.fonttype"] = 42
plt.rcParams["axes.spines.right"] = False
plt.rcParams["axes.spines.top"] = False

if __name__ == '__main__':
    plt.figure(figsize=(3.5, 3.5))

    df = pd.read_csv("results.csv")
    df = df[df["rep"] == 1]
    df["value"] = np.round(df["hash"] / df["sort"], 2)
    heatmap_data = df.pivot_table(index="S", columns="R", values="value")
    heatmap_data.columns = heatmap_data.index = ["$10^{4}$", "$10^{5}$", "$10^{6}$", "$10^{7}$", "$10^{8}$"]
    ax = sns.heatmap(heatmap_data, annot=True, cmap="BuPu", cbar=False, linewidths=0, fmt=".1f")

    plt.gca().invert_yaxis()

    plt.xlabel("R size", labelpad=10)
    plt.ylabel("S size", labelpad=10)

    for tick in ax.get_xticklabels() + ax.get_yticklabels():
        tick.set_fontsize(12)

    plt.savefig("heatmap.pdf", bbox_inches="tight")
