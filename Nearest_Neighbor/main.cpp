#include <iostream>
#include <vector>
#include <climits>
#include <map>
#include <deque>
    
class Node {
public:
    int city;
    Node() {
    }
    Node(int city) {
        this->city = city;
    }
    ~Node() {

    }
};

int get_cost(Node *a, Node *b, std::vector<std::vector<int>> tsp) {
    return tsp[a->city][b->city];
}

int main() {
    std::vector<std::vector<int>> tsp = {
        // A  B  C   D
        {-1, 10, 15, 20}, // A
        {10, -1, 35, 25}, // B
        {15, 35, -1, 30}, // C
        {20, 25, 30, -1}  // D
    };

    std::vector<Node*> cities;
    for (int x=0; x<tsp.size(); ++x) {
        cities.push_back(new Node(x));
    }

    std::map<Node*, bool> visited;
    std::deque<Node*> Q;
    std::deque<Node*> best_route;
    Node *current;
    int cost;
    int closest_cost;
    int optimal_cost = INT_MAX;

    for (auto city: cities) {
        visited.clear();
        Q.clear();
        
        visited[city] = true;
        Q.push_front(city);
        current = Q.front();
        cost = 0;

        while (Q.size() != cities.size()) {
            for (int i=0; i<cities.size(); ++i) {
                closest_cost = INT_MAX;
                if ((!visited[cities[i]])) {
                    std::cout <<  "current " << current->city+1 << " city " << i+1 << " cost " << get_cost(current, cities[i], tsp) << std::endl;
                    if (get_cost(current, cities[i], tsp) < closest_cost) {
                        Q.push_back(cities[i]);
                        closest_cost = get_cost(current, cities[i], tsp);
                        cost += closest_cost;
                        current = cities[i];
                    }
                }
                visited[cities[i]] = true;
            } 
        }
        
        std::cout << "Route cost: " << cost << std::endl; 
        if (cost < optimal_cost) {
            best_route = Q;
            optimal_cost = cost;
        }
    }

    std::vector<int> path;
    for (auto city: best_route) {
        path.push_back(city->city + 1);
        std::cout << path.back() << ',';
    }
    std::cout << std::endl << "Optimal cost: " << optimal_cost << std::endl;
    
    return 0;
}