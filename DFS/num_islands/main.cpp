#include <iostream>
#include <vector>
#include <stack>
using namespace std;

/* Recursive
void dfs(int row, int col, vector<vector<char>>& grid, vector<vector<int>> dirs) {
    if (row < 0 || col < 0 || row >= grid.size() || col >= grid[0].size() || grid[row][col] == '0') {
        return;
    }
    
    grid[row][col] = '0';
    
    for (const auto& dir: dirs) {
        int new_row = row + dir[0];
        int new_col = col + dir[1];
        dfs(new_row, new_col, grid, dirs);
    }
}

int numIslands(vector<vector<char>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    int num_islands = 0;
    
    vector<vector<int>> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            if (grid[i][j] == '1') {
                num_islands++;
                dfs(i, j, grid, dirs);
            }
        }
    }

    return num_islands;
}
*/

int numIslands(vector<vector<char>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    int num_islands = 0;
    
    vector<vector<int>> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            if (grid[i][j] == '0') {
                continue;
            }
            num_islands++;
            
            stack<pair<int, int>> S;
            S.push({i, j});

            while (!S.empty()) {
                auto [row, col] = S.top();
                S.pop();
                
                if (row < 0 || col < 0 || row >= n || col >= m || grid[row][col] == '0') {
                    continue;
                }
                
                grid[row][col] = '0';

                for (const auto& dir: dirs) {
                    int new_row = row + dir[0];
                    int new_col = col + dir[1];

                    S.push({new_row, new_col});
                }
            }
        }
    }

    return num_islands;
}

int main() {
    vector<vector<char>> grid;
    grid = {
        {'1', '1', '1', '1', '0'},
        {'1', '1', '0', '1', '0'},
        {'1', '1', '0', '0', '0'},
        {'0', '0', '0', '0', '0'}
    };

    int result = numIslands(grid);
    cout << result << endl;

    // Test Case 2
    grid = {
        {'1', '1', '0', '0', '0'},
        {'1', '1', '0', '0', '0'},
        {'0', '0', '1', '0', '0'},
        {'0', '0', '0', '1', '1'}
    };
    
    result = numIslands(grid);
    cout << result << endl;

    return 0;
}