#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <chrono>
#include <climits>
#include "json.hpp"
#include <random>
#include <math.h>
#include <algorithm>

#define INF INT_MAX;

using json = nlohmann::json;

struct Node {
    double x;
    double y;
    Node *prev;
    Node *next;
};

double distance(Node *one, Node *two) {
    return std::sqrt(std::pow((one->x - two->x), 2) + std::pow((one->y - two->y), 2));
}

bool check_collision(Node* near_node, double corr_x, double corr_y, std::vector<std::vector<double>> obstacles_x, std::vector<std::vector<double>> obstacles_y) {
    double min_x, min_y, max_x, max_y, m, y, x;
    bool decision;
    for (int i=0; i<obstacles_x.size(); i++) {
        min_x = *std::min_element(obstacles_x[i].begin(), obstacles_x[i].end());
        min_y = *std::min_element(obstacles_y[i].begin(), obstacles_y[i].end());
        max_x = *std::max_element(obstacles_x[i].begin(), obstacles_x[i].end());
        max_y = *std::max_element(obstacles_y[i].begin(), obstacles_y[i].end());

        // AABB
        if ((near_node->x <= min_x && corr_x <= min_x) || 
            (near_node->y <=min_y && corr_y <= min_y) ||
            (near_node->x >= max_x && corr_x >= max_x) ||
            (near_node->y >= max_y && corr_y >= max_y)) {
            
            decision = false;
        }

        m = (corr_y - near_node->y) / (corr_x - near_node->x);
        
        y = m * (min_x - near_node->x) + near_node->y;
        if (y > min_y && y < max_y) {
            decision = true;
        }

        y = m * (max_x - near_node->x) + near_node->y;
        if (y > min_y && y < max_y) {
            decision = true;
        }

        x = (min_y - near_node->y) / m + near_node->x;
        if (x > min_x && x < max_x) {
            decision = true;
        }
        
        x = (max_y - near_node->y) / m + near_node->x;
        if (x > min_x && x < max_x) {
            decision = true;
        }
    }

    return decision;
}

int main() {
    
    // Read map vars from config file
    std::ifstream f{"config.json"};
    json config = json::parse(f);

    double map_width = config["map_width"];
    double map_height = config["map_height"];

    double start[] = {config["start"]["x"], config["start"]["y"]};
    double goal[] = {config["goal"]["x"], config["goal"]["y"]};

    std::vector<std::vector<double>> obstacles_x, obstacles_y;

    for (auto x: config["obstacles"]["x"]) {
        std::vector<double> obs_temp;
        for (int i=0; i<x.size(); i++) {
            obs_temp.push_back(x[i]);
        }
        obstacles_x.push_back(obs_temp);
    } 
    for (auto y: config["obstacles"]["y"]) {
        std::vector<double> obs_temp;
        for (int i=0; i<y.size(); i++) {
            obs_temp.push_back(y[i]);
        }
        obstacles_y.push_back(obs_temp);
    }

    // Rapidly Exploring Random Trees (RRT)

    int max_iterations = config["max_iteration"];
    double max_distance = config["max_distance"];
    double goal_threshold = config["goal_threshold"];
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distrib(0, 100);

    /* Start algo */
    auto t0 = std::chrono::high_resolution_clock::now();

    Node *start_node = new Node;
    start_node->x = start[0];
    start_node->y = start[1];
    start_node->prev = nullptr;
    Node *goal_node = new Node({goal[0], goal[1]});
    
    std::vector<Node*> rrt_nodes;
    rrt_nodes.push_back(start_node);
    
    double dist, angle;
    bool collision;
    std::vector<std::pair<double, double>> optimal_path;
    for (int i=0; i < max_iterations; i++) {
        Node *random_node = new Node({(double)distrib(rng), (double)distrib(rng)});
        Node *near_node = new Node;

        int min_distance = INT_MAX;
        double corr_x = 0.0;
        double corr_y = 0.0;
        for (auto &node: rrt_nodes) {
            dist = distance(random_node, node);
            if (dist < min_distance) {
                min_distance = dist;
                near_node = node;
            }
        }

        angle = std::atan2(random_node->x - near_node->x, random_node->y - near_node->y) * 180 / M_PI;

        if (min_distance > max_distance) {
            corr_x = std::abs(near_node->x + std::cos(angle) * max_distance);
            corr_y = std::abs(near_node->y + std::sin(angle) * max_distance);
        }

        if (min_distance <= max_distance) {
            corr_x = random_node->x;
            corr_y = random_node->y;
        }

        collision = check_collision(near_node, corr_x, corr_y, obstacles_x, obstacles_y);
        
        if (collision == false) {
            rrt_nodes.push_back(random_node);
            
            random_node->x = corr_x;
            random_node->y = corr_y;

            near_node->next = random_node;
            random_node->prev = near_node;


            if (distance(random_node, goal_node) < goal_threshold) {
                std::cout << "Goal reached after " << i << " iterations" << std::endl;
                
                // Find shortest path
                while (random_node->prev != nullptr) {
                    random_node = random_node->prev;
                    optimal_path.push_back(std::pair(random_node->x, random_node->y));
                }
                break;
            }
        }
    }
    
    /* End algo */ 
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = t1 - t0;
    std::cout << "Time: " << duration.count() << "s" << std::endl;
    
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
    
    // Export visited paths
    std::ofstream visit_path{"visit.csv"};
    visit_path << "x" << "," << "y" << "," << "path" << '\n';

    std::vector<std::tuple<double, double, int>> explored_path;
    int path_num = 0;
    for (auto &node: rrt_nodes) {
        while (node->prev != nullptr) {
            node = node->prev;
            explored_path.push_back(std::tuple(node->x, node->y, path_num));
        }
        path_num++;
    }
    for (const auto x: explored_path) {
        if (x == explored_path.back()) {
            visit_path << std::get<0>(x) << ',' <<  std::get<1>(x) << ',' << std::get<2>(x);
        }
        else {
            visit_path << std::get<0>(x) << ',' <<  std::get<1>(x) << ',' << std::get<2>(x) << '\n';
        }
    }

    return 0;
}