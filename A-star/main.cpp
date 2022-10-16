#include <iostream>
#include "node.h"
#include <vector>
#include <queue>
#include <map>
#include <fstream>
#include <chrono>
#include <climits>
#include "json.hpp"

#define INF INT_MAX;

using json = nlohmann::json;

// Cost heursitic of A*, pointer to node
typedef std::pair<int, Node*> H;

int get_cost(Node *one, Node *two) {
    int dist = abs(one->x - two->x) + abs(one->y - two->y);
    if ((one->obstacle) || (two->obstacle)) {
        return INF;
    }
    else {
        return dist;
    }
}

int main(int argc, char const *argv[]) {

    // Read map vars from config file
    std::ifstream f{"config.json"};
    json config = json::parse(f);

    int map_width = config["map_width"];
    int map_height = config["map_height"];

    int start[] = {config["start"]["x"], config["start"]["y"]};
    int goal[] = {config["goal"]["x"], config["goal"]["y"]};

    std::vector<int> obstacles_x, obstacles_y;

    for (auto x: config["obstacles"]["x"]) {
        obstacles_x.push_back(x);
    } 
    for (auto y: config["obstacles"]["y"]) {
        obstacles_y.push_back(y);
    }

    // Create map
    std::vector<std::vector<Node*>> map(map_height, std::vector<Node*> (map_width));

    // Create grid of nodes
    for (int x=0; x<map_width; ++x) {
        for (int y=0; y<map_height; ++y) {
            map[y][x] = new Node(x, y);
        }
    }

    // Create neighbors
    for (int x=0; x<map_width; ++x) {
        for (int y=0; y<map_height; ++y) {
            if (y < map_height - 1 ) {
                map[y][x]->neighbors.push_back(map[y+1][x]);
            }
            if (x < map_width - 1) {
                map[y][x]->neighbors.push_back(map[y][x+1]);
            }
            if (y > 0) {
                map[y][x]->neighbors.push_back(map[y-1][x]);
            }
            if (x > 0) {
                map[y][x]->neighbors.push_back(map[y][x-1]);
            }
        }
    }

    // Label obstacles
    for (int i=0; i<obstacles_x.size(); ++i) {
        map[obstacles_y[i]][obstacles_x[i]]->obstacle = 1;
    }

    // Start and Goal
    map[start[1]][start[0]]->start = 1;
    map[goal[1]][goal[0]]->goal = 1;
    
    // Cost
    map[start[1]][start[0]]->cost = 0;

    // Export graph
    std::vector<std::vector<int>> graph;
    for (int x=0; x<map_width; ++x) {
        std::vector<int> row;
        for (int y=0; y<map_height; ++y) {
            row.push_back(map[y][x]->obstacle);
        }
        graph.push_back(row);
    }

    json graph_json(graph);

    std::ofstream out_graph{"graph.json"};
    out_graph << graph_json;
    
    // A* algorithm

    /* Start algo */
    auto t0 = std::chrono::high_resolution_clock::now();

    std::priority_queue<H, std::vector<H>, std::greater<H>> Q;
    std::map<Node*, int> visited, unvisited;
    Node *start_node = map[start[1]][start[0]];
    Node *goal_node = map[goal[1]][goal[0]];
    Node *current;

    // Heuristic for priority queue
    Q.push(std::make_pair(start_node->cost+get_cost(start_node, goal_node), start_node));

    bool goal_found = false;
    while ((Q.size()) && (!goal_found)) {
        current = Q.top().second;
        Q.pop();

        for (Node *n: current->neighbors) {
            if (!(n->obstacle) && !(visited[n])) {
                if (current->cost + get_cost(current, n) < n->cost) {
                    n->cost = current->cost + get_cost(current, n);
                    n->parent = current;
                }

                if (!unvisited[n]) {
                    // Heuristic to move unvisited neighbors closer to the top of priority queue
                    Q.push(std::make_pair(n->cost + get_cost(n, goal_node), n));
                    ++unvisited[n];
                }
            }

            if (n == goal_node) {
                std::cout << "Goal found!" << std::endl;
                goal_found ^= true;
                break;
            }
        }

        if (visited[current] == 0) {
            ++visited[current];
        }
    }

    // Find shortest path
    std::vector<std::pair<int, int>> optimal_path;
    current = goal_node;
    while (current != start_node) {
        if (current == nullptr) {
            std::cout << "NULL encountered" << std::endl;
            return 0;
        }

        optimal_path.push_back(std::pair(current->x, current->y));
        current = current->parent;
    }
    optimal_path.push_back(std::pair(current->x, current->y));
    
    /* End algo */ 
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = t1 - t0;
    std::cout << "Time: " << duration.count() << "s" << std::endl;

    // Delete ptrs in map
    for (int x=0; x<map_width; ++x) {
        for (int y=0; y<map_height; ++y) {
            if (map[y][x] == nullptr) {
                // Do nothing
            }
            else {
                delete map[y][x];
            }
        }
    }
    map.clear();

    // Export shortest path
    std::ofstream out_path{"path.csv"};
    out_path << "x" << "," << "y" << '\n';
    for (const auto x: optimal_path) {
        if (x == optimal_path.back()) {
            out_path << x.first << ',' <<  x.second;
        }
        else {
            out_path << x.first << ',' <<  x.second << '\n';
        }
    }
    
    return 0;
}