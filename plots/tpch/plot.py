import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

plt.rcParams["font.size"] = 14
plt.rcParams["font.family"] = "Times New Roman"
plt.rcParams["pdf.fonttype"] = 42
plt.rcParams["ps.fonttype"] = 42
plt.rcParams["axes.spines.right"] = False
plt.rcParams["axes.spines.top"] = False


def scatter_plot(df):
	plt.figure(figsize=(3, 3))

	df = df[["DuckDB", "EJCO"]]
	df /= 1000
	eye_line = [np.min(df) / 1.2, np.max(df) * 1.2]
	plt.plot(eye_line, eye_line, color="lightgray", lw=0.5)
	x_values = df["DuckDB"].values
	y_values = df["EJCO"].values
	plt.scatter(x_values, y_values, color="black", s=10, label="Binary Join", zorder=3)

	plt.xscale("log")
	plt.yscale("log")
	plt.xlabel(f"Free Join framework (s)")
	plt.ylabel(f"EJCO (s)")
	plt.xlim(eye_line)
	plt.ylim(eye_line)
	plt.legend(loc="upper center", ncol=1, bbox_to_anchor=(0.5, 1.25))


def bar_plot(df):
	df["Query"] = df["Query"].apply(lambda x: f"Q{x}")
	df[["DuckDB", "EJCO"]] /= 1000

	colors = ["gray", "black"]
	ax = df.plot.bar(logy=True, color=colors, edgecolor="white", lw=0)
	ax.set_ylabel("Run Time (s)", loc="top", rotation=0, labelpad=-75)
	ax.set_xticklabels(df["Query"], fontsize=12)
	ax.legend(loc="upper center", ncol=2, bbox_to_anchor=(0.5, 1.17), frameon=False)
	ax.grid(axis="y", linestyle="--", color="lightgray")
	ax.set_axisbelow(True)
	ax.figure.set_size_inches(7, 3)


if __name__ == '__main__':
	df = pd.read_csv("results.csv")

	# scatter_plot(df)
	bar_plot(df)

	plt.savefig("tpch.pdf", bbox_inches="tight")
