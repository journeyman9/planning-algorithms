#include <iostream>
#include <vector>
#include <stack>

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

void pre_order_helper(TreeNode* root, std::vector<int> &path) {
    if (root == nullptr) {
        return;
    }
    path.push_back(root->val);
    pre_order_helper(root->left, path);
    pre_order_helper(root->right, path);
}

std::vector<int> pre_order(TreeNode* root) {
    std::vector<int> path;
    pre_order_helper(root, path);
    return path;
}

void in_order_helper(TreeNode* root, std::vector<int> &path) {
    if (root == nullptr) {
        return;
    }
    in_order_helper(root->left, path);
    path.push_back(root->val);
    in_order_helper(root->right, path);
}

std::vector<int> in_order(TreeNode* root) {
    std::vector<int> path;
    in_order_helper(root, path);
    return path;
}

void post_order_helper(TreeNode* root, std::vector<int> &path) {
    if (root == nullptr) {
        return;
    }
    post_order_helper(root->left, path);
    post_order_helper(root->right, path);
    path.push_back(root->val);
}

std::vector<int> post_order(TreeNode* root) {
    std::vector<int> path;
    post_order_helper(root, path);
    return path;
}

std::vector<int> pre_order_iterative(TreeNode* root) {
    std::vector<int> path;
    if (root == nullptr) {
        return path;
    }

    std::stack<TreeNode*> S;
    S.push(root);

    TreeNode *curr;

    while (!S.empty()) {
        curr = S.top();
        S.pop();
        path.push_back(curr->val);

        if (curr->right != nullptr) {
            S.push(curr->right);
        }
        
        if (curr->left != nullptr) {
            S.push(curr->left);
        }
    }

    return path;
}

std::vector<int> in_order_iterative(TreeNode* root) {
    std::vector<int> path;
    if (root == nullptr) {
        return path;
    }

    std::stack<TreeNode*> S;
    TreeNode *curr = root;

    while (curr != nullptr || !S.empty()) {
        while (curr != nullptr) {
            S.push(curr);
            curr = curr->left;
        }
        curr = S.top();
        S.pop();
        path.push_back(curr->val);

        curr = curr->right;
    }

    return path;
}

std::vector<int> post_order_iterative(TreeNode* root) {
    std::vector<int> path;
    if (root == nullptr) {
        return path;
    }

    std::stack<TreeNode*> S1, S2;
    TreeNode *curr = root;
    S1.push(curr);

    while (!S1.empty()) {
        curr = S1.top();
        S1.pop();
        S2.push(curr);

        if (curr->left != nullptr) {
            S1.push(curr->left);
        }
        if (curr->right != nullptr) {
            S1.push(curr->right);
        }
    }

    while (!S2.empty()) {
        curr = S2.top();
        S2.pop();
        path.push_back(curr->val);
    }

    return path;
}

int main() {
    int tree[] = {3, 9, 20, NULL, NULL, 15, 7};
    int n = sizeof(tree) / sizeof(tree[0]);
    TreeNode *root = build_tree(tree, 0, n);
    
    // Recursive
    //std::vector<int> path = pre_order(root);
    //std::vector<int> path = in_order(root);
    //std::vector<int> path = post_order(root);

    // Iterative
    //std::vector<int> path = pre_order_iterative(root);
    //std::vector<int> path = in_order_iterative(root);
    std::vector<int> path = post_order_iterative(root);

    for (auto x: path) {
        std::cout << x << ",";
    }
    std::cout << std::endl;
    return 0;
}