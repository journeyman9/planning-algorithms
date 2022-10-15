#pragma once
#include <string>
#include <vector>

class Node {
public:
    int obstacle, start, goal;
    int x, y;
    int cost;

    std::vector<Node*> neighbors;
    Node* parent;

    Node();
    Node(int x, int y);
    Node(int, int, std::string);
    ~Node();
};