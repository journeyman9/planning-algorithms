#include <iostream>
#include <vector>
#include <deque>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(): val(0), left(nullptr), right(nullptr) {
    }
    TreeNode(int x): val(x), left(nullptr), right(nullptr) {
    }
    TreeNode(int x, TreeNode *left, TreeNode *right): val(x), left(left), right(right) {
    }
};

TreeNode* new_node(int val) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->val = val;
    node->left = node->right = nullptr;
    return (node);
}

TreeNode* build_tree(int tree[], int count, int n) {
    TreeNode *root = nullptr;
    if (count < n && tree[count] != NULL) {
        root = new_node(tree[count]);
        root->left = build_tree(tree, 2*count+1, n);
        root->right = build_tree(tree, 2*count+2, n);
    }
    return root;
}

std::vector<std::vector<int>> bfs_order(TreeNode* root) {
    std::vector<std::vector<int>> path;
    
    if (root == nullptr) {
        return path;
    }
    
    std::deque<TreeNode*> Q;
    Q.push_back(root);
    
    std::vector<int> level;
    TreeNode *curr;      

    while (!Q.empty()) {
        int n = Q.size();
        for (int i=0; i<n; i++) {
            curr = Q.front();
            Q.pop_front();
            level.push_back(curr->val);
            
            if (curr->left != nullptr) {
                Q.push_back(curr->left);
            }
            if (curr->right != nullptr) {
                Q.push_back(curr->right);
            }
        }
        path.push_back(level);
        level.clear();
    }
    return path;
}

int main() {
    int tree[] = {3, 9, 20, NULL, NULL, 15, 7};
    int n = sizeof(tree) / sizeof(tree[0]);
    TreeNode *root = build_tree(tree, 0, n);
    
    std::vector<std::vector<int>> path = bfs_order(root);

    for (auto x: path) {
        for (auto y: x) {
            std::cout << y << ",";
        }
        std::cout << std::endl;
    }
    return 0;
}