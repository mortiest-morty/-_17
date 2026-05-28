#pragma once
#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <vector>
#include <queue>
#include <sstream>
#include <limits>
#include <cmath>
#include <algorithm>
#include <functional>

int getNumber();
std::string getText();

// Класс: узел дерева
template <typename T>
class Node {
private:
    T key;
    Node* left;
    Node* right;

public:
    Node(T key) : key(key), left(nullptr), right(nullptr) {}

    T getKey() const { return key; }
    Node* getLeft() const { return left; }
    Node* getRight() const { return right; }

    void setKey(T value) { key = value; }
    void setLeft(Node* l) { left = l; }
    void setRight(Node* r) { right = r; }

    template <typename U> friend class binaryTree;
};

// Класс: бинарное дерево выражений
template <typename T>
class binaryTree {
private:
    Node<T>* root;

    void clear(Node<T>* node);
    Node<T>* buildFromPrefix(const std::vector<T>& expr, int& index);
    int evaluate(Node<T>* node) const;
    Node<T>* optimizeNode(Node<T>* node);
    void printTreeRec(Node<T>* node, int level) const;
    char getOpSymbol(T code) const;
    void saveTreeRec(Node<T>* node, int level, std::ofstream& file) const;

public:
    binaryTree() : root(nullptr) {}
    ~binaryTree() { clear(root); }

    Node<T>* getRoot() const { return root; }
    void buildTree(const std::vector<T>& expr);
    void processTree();
    void printTree() const;
    void saveTreeToFile(const std::string& filename) const;
    bool isEmpty() const { return root == nullptr; }
    void clear() { clear(root); root = nullptr; }

    static char codeToOperator(T code) {
        switch (code) {
        case -1: return '+';
        case -2: return '-';
        case -3: return '*';
        case -4: return '/';
        case -5: return '%';
        case -6: return '^';
        default: return '?';
        }
    }
};

void task_1();
void task_2();
void solve();

#endif
