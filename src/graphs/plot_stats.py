#!/usr/bin/env python3.9
# coding=utf-8

# file name: plot_stats.py
#
# Script to analyze result data of restaurant with delivery system modeled by Petri net model.
# project: IMS
#
# authors: David Chocholatý (xchoch08), David Mihola (xmihol00), FIT BUT

from matplotlib import pyplot as plt
from matplotlib import dates as matdates
import pandas as pd
import seaborn as sns
import numpy as np
import os
import sys


def get_dataframe(filename: str = "accidents.pkl.gz", verbose: bool = False) -> pd.DataFrame:
    """
    Gets data frame from specified data file.

    Args:
        filename (str): Name of the file with the data.
        verbose (bool): Whether to print informational messages.

    Returns:
        pd.DataFrame: Data in a data frame.
    """
    df = pd.read_csv(filename)
    print(df)

    for column_name in df.columns:
        df[column_name] = pd.to_numeric(df[column_name])

    return df


def plot_graph(df: pd.DataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Plot graph depicting restaurant driver utility per minute.
    Args:
        df (pd.DataFrame): Data frame to plot.
        fig_location (str): Location where to store the generated figure. If None, do not store at all.
        show_figure (bool): Whether to show the generated figure.
    """
    vehicle_type_map = {
        "cars": "auta",
        "bikes": "motorky",
    }

    df = df.melt(id_vars="time", value_vars=["cars", "bikes"], var_name='type',
                 value_name='count')
    df.loc[:, "type"] = df["type"].map(vehicle_type_map).astype('category')

    sns.set_theme(style="whitegrid")
    sbg = sns.relplot(x="time", y="count", hue="type", data=df, kind="line")
    plt.yticks(df.loc[:, "count"].unique())
    plt.xticks([i for i in range(0, df.loc[:, "time"].max() + 60, 60)])
    sbg.set_axis_labels("Čas od začátku směny (min)", "Počet rozvážejících řidičů")
    sbg.despine()
    sbg._legend.set_title("Druh vozidla")
    # sbg.tight_layout()
    # fig = sbg.fig
    # fig.suptitle("Povětrnostní podmínky v jednotlivých měsících")
    axis = sbg.axes.flat
    # xformatter = matdates.DateFormatter("%m/%y")
    # axis[0].xaxis.set_major_formatter(xformatter)
    for ax in axis:
        ax.tick_params(bottom=True, left=True, labelleft=True, labelbottom=True)

    if fig_location:
        sbg.savefig(fig_location)

    if show_figure:
        plt.show()


if __name__ == "__main__":
    """
    Runs the main script operations when run as a standalone script.
    """
    filename = sys.argv[1]
    df = get_dataframe(f"{filename}.csv", True)
    plot_graph(df, fig_location=f"{filename}.png", show_figure=False)
