#ifndef CONTROLLERNODE_H
#define CONTROLLERNODE_H

#include <zmq.hpp>
#include <string>
#include <unordered_map>
#include <thread>
#include <memory>
#include <queue>
#include <iostream>

struct TreeNode {
    int id;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int id) : id(id), left(nullptr), right(nullptr) {}
};

class BalancedBinaryTree {
public:
    BalancedBinaryTree() : root(nullptr) {}

    void insert(int id, int parent = -1) {
        if (parent == -1) {
            root = insertBalanced(root, id);
        } else {
            TreeNode* parentNode = findNode(root, parent);
            if (parentNode) {
                if (!parentNode->left) {
                    parentNode->left = new TreeNode(id);
                } else if (!parentNode->right) {
                    parentNode->right = new TreeNode(id);
                } else {
                    std::cerr << "Error: Parent node already has two children" << std::endl;
                }
            } else {
                std::cerr << "Error: Parent node not found" << std::endl;
            }
        }
    }

    // Поиск узла по id
    TreeNode* findNode(int id) {
        return findNode(root, id);
    }

private:
    TreeNode* root;

    // Рекурсивная вставка в самое наименьшее поддерево
    TreeNode* insertBalanced(TreeNode* node, int id) {
        if (node == nullptr) {
            return new TreeNode(id);
        }

        // Вычисляем высоту левого и правого поддеревьев
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);

        // Добавляем в самое наименьшее поддерево
        if (leftHeight <= rightHeight) {
            node->left = insertBalanced(node->left, id);
        } else {
            node->right = insertBalanced(node->right, id);
        }

        return node;
    }

    // Рекурсивный поиск узла
    TreeNode* findNode(TreeNode* node, int id) {
        if (node == nullptr || node->id == id) {
            return node;
        }

        TreeNode* leftResult = findNode(node->left, id);
        if (leftResult) {
            return leftResult;
        }

        return findNode(node->right, id);
    }

    // Вычисление высоты дерева
    int getHeight(TreeNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return 1 + std::max(getHeight(node->left), getHeight(node->right));
    }
};

class ControllerNode {
public:
    ControllerNode();
    void start();
    void createNode(int id, int parent = -1);
    void execCommand(int id, const std::string& params);
    void pingNode(int id);
    void heartbit(int time);

private:
    void sendCommand(const std::string& command);
    void launchWorkerNode(int id, int parentId = -1);

    zmq::context_t context_;
    zmq::socket_t socket_;
    std::unordered_map<int, std::thread> workerThreads_;
    BalancedBinaryTree tree_;
};


#endif