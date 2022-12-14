"""
Plot grid of path chosen by planner. The start or source is colored blue.
The goal or destination is yellow. Obstacles are black.

python plot_path.py
"""

__author__ = "Journey McDowell"

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import json
import argparse
import pdb

if __name__ == "__main__":
    # Load start and goal
    f = open("config.json")
    config = json.load(f)

    min_width = config["map_width"]
    min_height = config["map_height"]
    start = [config["start"]["y"], config["start"]["x"]]
    goal = [config["goal"]["y"], config["goal"]["x"]]

    # Load graph
    f = open("graph.json")
    graph = json.load(f)
    graph = np.array(graph)

    # Load chosen path
    df = pd.read_csv("path.csv")

    # Load visited paths
    df_visit = pd.read_csv("visit.csv")

    # Plot
    fig, ax = plt.subplots(1, 1)
    ax.scatter(df_visit["x"], df_visit["y"], color='red', alpha=0.05)
    ax.imshow(graph, cmap=plt.cm.Dark2)
    ax.scatter(start[1], start[0], marker='*', color='blue', s=200)
    ax.scatter(goal[1], goal[0], marker='*', color='yellow', s=200)
    ax.plot(df["x"], df["y"], color='black')
    plt.show()