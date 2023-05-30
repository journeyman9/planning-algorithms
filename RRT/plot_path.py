"""
Plot grid of path chosen by planner. The start or source is colored blue.
The goal or destination is yellow. Obstacles are black.

python plot_path.py
"""

__author__ = "Journey McDowell"

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
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

    # Load obstacles
    obstacles_x = config["obstacles"]["x"]
    obstacles_y = config["obstacles"]["y"]

    # Load chosen path
    df = pd.read_csv("path.csv")

    # Load visited paths
    df_visit = pd.read_csv("visit.csv")

    # Plot
    fig, ax = plt.subplots(1, 1)

    # Explored
    for i in range(len(df_visit['path'].unique())):
        df_temp = df_visit[df_visit['path'] == i]
        ax.plot(df_temp["x"], df_temp["y"], color='brown', alpha=0.25)

    # Obstacles
    for i in range(len(obstacles_x)):
        ax.plot(obstacles_x[i], obstacles_y[i], color='red')

    ax.scatter(start[1], start[0], marker='*', color='blue', s=200)
    ax.scatter(goal[1], goal[0], marker='*', color='yellow', s=200)
    ax.plot(df["x"], df["y"], color='black')
    plt.show()