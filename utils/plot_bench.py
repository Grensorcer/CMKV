#!/usr/bin/env python
from argparse import ArgumentParser
import pandas as pd

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("bench_name", type=str)
    args = parser.parse_args()
    bench_path = "utils/bench/" + args.bench_name + "/"

    times = pd.read_csv(bench_path + "chrono.csv", sep=";", index_col=0)
    successes = pd.read_csv(bench_path + "success.csv", sep=";", index_col=0)
    times.columns = times.columns.str.strip()
    successes.columns = successes.columns.str.strip()

    times.drop(["nb_iter"], axis=1, inplace=True)
    times = times.iloc[:, :-1]
    successes.drop(["nb_iter"], axis=1, inplace=True)
    successes = successes.iloc[:, :-1]

    fig = times.plot.bar(
        rot=0, logy=True, xlabel="Methods", ylabel="microseconds"
    ).get_figure()
    fig.savefig(bench_path + "chrono.png")
    fig = successes.plot.bar(rot=0).get_figure()
    fig.savefig(bench_path + "success.png")
