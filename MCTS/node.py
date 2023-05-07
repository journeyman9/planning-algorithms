import random
import numpy as np

class Node:
    def __init__(self, node_id=None):
        self.left = None
        self.right = None
        self.node_id = node_id
        self.value = None

    def __repr__(self):
        return "node@" + str(self.node_id)

    def __str__(self):
        return "node@" + str(self.node_id)

    def is_terminal(self):
        if self.left is None and self.right is None:
            return True
        return False

    def find_children(self):
        if self.is_terminal():
            return {}
        return {self.left, self.right}

    def find_children_list(self):
        """This makes sure left, right order instead of an unsorted set"""
        if self.is_terminal():
            return []
        return [self.left, self.right]

    def find_random_child(self):
        if self.is_terminal():
            return None
        if random.choice([0, 1]) == 0:
            return self.left
        else:
            return self.right

    def reward(self):
        return self.value
