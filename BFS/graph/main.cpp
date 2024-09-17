#include <iostream> 
#include <vector>
#include <unordered_map>
#include <string>
#include <deque>
using namespace std;

bool validPath(int n, vector<vector<int>>& edges, int source, int destination) {
    unordered_map<int, vector<int>> graph;
    for (auto& edge: edges) {
        graph[edge[0]].push_back(edge[1]);
        graph[edge[1]].push_back(edge[0]);
    }

    vector<bool> visited(n, false);
    deque<int> Q;

    Q.push_back(source);
    visited[source] = true;

    int curr;

    while (!Q.empty()) {
        curr = Q.front();
        Q.pop_front();
        if (curr == destination) {
            return true;
        }

        for (auto& neighbor: graph[curr]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                Q.push_back(neighbor);
            }
        }
    }
    return false;

}

int main() {
    int test_n = 3;
    vector<vector<int>> edges{{0, 1}, {1, 2}, {2, 0}};
    int source = 0, destination = 2;

    bool result = validPath(test_n, edges, source, destination);
    cout << result << endl;
    
    test_n = 6;
    vector<vector<int>> edges2{{0, 1}, {0, 2}, {3, 5}, {5, 4}, {4, 3}};
    source = 0, destination = 5;

    result = validPath(test_n, edges2, source, destination);
    cout << result << endl;

    return 0;
}