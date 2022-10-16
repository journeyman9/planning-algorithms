#include "node.h"
#include <climits>

#define INF INT_MAX;

Node::Node() {
    this->obstacle = 0;
    this->start = 0;
    this->goal = 0;
    this->parent = nullptr;
    this->cost = INF;
}

Node::Node(int x, int y) {
    this->x = x;
    this->y = y;
    this->obstacle = 0;
    this->start = 0;
    this->goal = 0;
    this->parent = nullptr;
    this->cost = INF;
}

Node::~Node() {
}