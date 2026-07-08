#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
    vector<vector<int>> dirs({{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}});
    
    int n = grid.size();
    if (grid[0][0] != 0 || grid[n-1][n-1] != 0) {
        return -1;
    }
    
    queue<vector<int>> Q;
    Q.push({0, 0, 1}); // row, col, dist
    
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    visited[0][0] = true;
    vector<int> cell;
    int row, col, dist;
    int new_row, new_col;
    vector<vector<int>> neighbors;
    
    while (!Q.empty()) {
        cell = Q.front();
        Q.pop();
        row = cell[0];
        col = cell[1];
        dist = cell[2];
        
        if (row == n-1 && col == n-1 ) {
            return dist;
        }
        
        // Get neighbors
        neighbors.clear();
        for (const auto& dir: dirs) {
            new_row = row + dir[0];
            new_col = col + dir[1];
            if (new_row < 0 || new_col < 0 || new_row >= n || new_col >= n || grid[new_row][new_col] != 0) {
                continue;
            }
            neighbors.push_back({new_row, new_col});
        }
        
        for (const auto& neighbor: neighbors) {
            int neighbor_row = neighbor[0];
            int neighbor_col = neighbor[1];
            if (visited[neighbor_row][neighbor_col]) {
                continue;
            }
            visited[neighbor_row][neighbor_col] = true;
            Q.push({neighbor_row, neighbor_col, dist+1});
        }
        
    }

    return -1;
}

int main() {
    // Case 1
    vector<vector<int>> grid({{0, 1}, {1, 0}});
    int result = shortestPathBinaryMatrix(grid);
    cout << result << endl;
    
    // Case 2
    grid = {
        {0, 0, 0},
        {1, 1, 0},
        {1, 1, 0},
    };
    result = shortestPathBinaryMatrix(grid);
    cout << result << endl;
    
    // Case 3
    grid = {
        {1, 0, 0},
        {1, 1, 0},
        {1, 1, 0}
    };
    result = shortestPathBinaryMatrix(grid);
    cout << result << endl;
    return 0;
}