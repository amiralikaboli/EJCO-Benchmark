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

if __name__ == "__main__":
    plt.figure(figsize=(3.5, 3.5))

    df = pd.read_csv("results.csv")
    df = df[df["rep"] == 1]
    df["value"] = np.round(df["hash"] / df["sort"], 2)
    heatmap_data = df.pivot_table(index="S", columns="R", values="value")
    heatmap_data.columns = heatmap_data.index = [f"$10^{i}$" for i in range(5, 10)]
    rows_na = set(heatmap_data.columns[heatmap_data.isna().any(axis=1)])
    cols_na = set(heatmap_data.columns[heatmap_data.isna().any(axis=1)])
    heatmap_data.drop(rows_na, axis=0, inplace=True)
    heatmap_data.drop(cols_na, axis=1, inplace=True)
    ax = sns.heatmap(
        heatmap_data, annot=True, cmap="BuPu", cbar=False, linewidths=0, fmt=".1f"
    )

    plt.gca().invert_yaxis()

    plt.xlabel("R size", labelpad=10)
    plt.ylabel("S size", labelpad=10)

    for tick in ax.get_xticklabels() + ax.get_yticklabels():
        tick.set_fontsize(12)

    plt.savefig("heatmap.pdf", bbox_inches="tight")
