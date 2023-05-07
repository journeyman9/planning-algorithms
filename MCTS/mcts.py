"""
Typical Monte Carlo Tree Search
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import collections
import math
import pdb

class MCTS:
    def __init__(self, exploration_weight=1.0):
        self.Q = collections.defaultdict(float) # Total reward of each node
        self.N = collections.defaultdict(float) # Total visit count for each node
        self.children = dict() # Key: explored node, Val: is set of children
        self.exploration_weight = exploration_weight

    def select(self, node):
        # Find an unexplored child (leaf) node by tree traversal
        path = []
        while True:
            path.append(node)
            if node not in self.children or not self.children[node]:
                return path
            unexplored = self.children[node] - self.children.keys()
            if unexplored:
                n = unexplored.pop()
                path.append(n)
                return path
            node = self._uct_select(node)

    def expand(self, node):
        # Add new child node to tree via node that was selected
        if node in self.children:
            return
        self.children[node] = node.find_children()

    def simulate(self, node):
        # rollout simulation with reward accumulated for each step using random actions
        while True:
            if node.is_terminal():
                return node.reward()
            node = node.find_random_child()

    def backup(self, path, reward):
        # Update value of ancestor to leaf node with accumulated rewards 
        for node in reversed(path):
            self.N[node] += 1
            self.Q[node] += reward

    def _uct_select(self, node):
        # Select a child of node, balance exploration and exploitation
        is_all_children_expanded = all(n in self.children for n in self.children[node])
        if not is_all_children_expanded:
            raise ValueError("Can only select from fully expanded node")

        log_N_parent = math.log(self.N[node])
        def uct(n):
            return (self.Q[node] / self.N[node]) + self.exploration_weight * math.sqrt(log_N_parent / self.N[node])

        return max(self.children[node], key=uct)
    
    def run(self, node, num_rollout):
        # Run an iteration of select->expand->rollout->backup
        path = self.select(node)
        leaf = path[-1]
        self.expand(leaf)
        reward = 0
        for i in range(num_rollout):
            reward += self.simulate(leaf)
        self.backup(path, reward)
    
    def policy(self, node):
        # Choose the best successor of node, i.e action
        if node.is_terminal():
            raise RuntimeError(f"policy called on a terminal node {node}")

        if node not in self.children:
            return node.find_random_child()

        def score(n):
            if self.N[n] == 0:
                return float("-inf") # Avoid unseen moves
            return self.Q[n] / self.N[n] # Average reward
            
        return max(self.children[node], key=score)
