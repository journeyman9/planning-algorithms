from node import Node
from mcts import MCTS
import argparse
import yaml
import random
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import pdb

def make_binary_tree(depth=12):
    all_nodes = []
    for i in range(depth + 1):
        nodes_at_depth = []
        num_of_nodes = pow(2, i)
        for j in range(num_of_nodes):
            nodes_at_depth.append(Node(str(i) + "_" + str(j)))
        all_nodes.append(nodes_at_depth)

    leaf_nodes_dict = dict()
    for level, nodes in enumerate(all_nodes):
        for loc, n in enumerate(nodes):
            if level >= len(all_nodes) - 1:
                # Assign reward value to leaf nodes of the tree
                n.value = random.uniform(0, 100)
                leaf_nodes_dict[n] = n.value
            else:
                left = all_nodes[level + 1][2 * loc]
                right = all_nodes[level + 1][2 * loc + 1]
                n.left = left
                n.right = right
    root = all_nodes[0][0]
    return root, leaf_nodes_dict

if __name__ == "__main__":
    with open("config.yml", "r") as file:
        config = yaml.safe_load(file)

    depth = config["depth"]
    num_iter = config["num_iter"]
    num_rollout = config["num_rollout"]
    exploration_weight = config["exploration_weight"]

    # Build tree with random values on leaf nodes
    root, leaf_nodes_dict = make_binary_tree(depth=depth)

    # Calculate ground truth
    leaf_nodes_dict_sorted = sorted(leaf_nodes_dict.items(), key=lambda x: x[1], reverse=True)
    print("Expected (max) leaf node: {}, value: {}".format(leaf_nodes_dict_sorted[0][0],
                                                           leaf_nodes_dict_sorted[0][1]))

    # Use MCTS
    mcts = MCTS(exploration_weight=exploration_weight)
    while True:
        for _ in range(num_iter):
            mcts.run(root, num_rollout=num_rollout)
        
        root = mcts.policy(root)

        if root.is_terminal():
            print("Found optimal (max) leaf node: {}, value: {}".format(root, root.value))
            break