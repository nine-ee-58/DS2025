#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

// 二叉树节点模板类
template<typename T>
class BinNode {
public:
    T data;                    // 数据域
    BinNode<T>* left;          // 左子节点
    BinNode<T>* right;         // 右子节点
    BinNode<T>* parent;        // 父节点

    // 构造函数
    BinNode(const T& d = T(), BinNode<T>* l = nullptr, BinNode<T>* r = nullptr, BinNode<T>* p = nullptr)
        : data(d), left(l), right(r), parent(p) {}

    // 判断是否为叶子节点
    bool isLeaf() const {
        return !left && !right;
    }

    // 判断是否为左子节点
    bool isLeftChild() const {
        return parent && parent->left == this;
    }

    // 判断是否为右子节点
    bool isRightChild() const {
        return parent && parent->right == this;
    }

    // 获取兄弟节点
    BinNode<T>* getSibling() const {
        if (!parent) return nullptr;
        return (isLeftChild()) ? parent->right : parent->left;
    }
};

// 二叉树模板类
template<typename T>
class BinTree {
protected:
    BinNode<T>* root;      // 根节点
    int _size;             // 节点总数

    // 释放以指定节点为根的子树
    void releaseSubtree(BinNode<T>* node) {
        if (!node) return;
        releaseSubtree(node->left);
        releaseSubtree(node->right);
        delete node;
    }

    // 复制以指定节点为根的子树
    BinNode<T>* copySubtree(BinNode<T>* node, BinNode<T>* parent = nullptr) {
        if (!node) return nullptr;
        
        BinNode<T>* newNode = new BinNode<T>(node->data, nullptr, nullptr, parent);
        newNode->left = copySubtree(node->left, newNode);
        newNode->right = copySubtree(node->right, newNode);
        
        return newNode;
    }

public:
    // 构造函数
    BinTree() : root(nullptr), _size(0) {}
    
    // 带根节点值的构造函数
    explicit BinTree(const T& value) : _size(1) {
        root = new BinNode<T>(value);
    }

    // 拷贝构造函数
    BinTree(const BinTree<T>& other) : root(nullptr), _size(0) {
        if (other.root) {
            root = copySubtree(other.root);
            _size = other._size;
        }
    }

    // 赋值运算符
    BinTree<T>& operator=(const BinTree<T>& other) {
        if (this != &other) {
            clear();
            if (other.root) {
                root = copySubtree(other.root);
                _size = other._size;
            }
        }
        return *this;
    }

    // 析构函数
    ~BinTree() {
        clear();
    }

    // 清空树
    void clear() {
        releaseSubtree(root);
        root = nullptr;
        _size = 0;
    }

    // 获取根节点
    BinNode<T>* getRoot() const { return root; }
    
    // 设置根节点
    void setRoot(BinNode<T>* node) { 
        root = node; 
        if (root) root->parent = nullptr;
    }

    // 获取节点总数
    int size() const { return _size; }
    
    // 判断树是否为空
    bool empty() const { return _size == 0; }

    // 获取树的高度
    int height() const { return height(root); }
    
    // 获取以指定节点为根的子树高度
    int height(BinNode<T>* node) const {
        if (!node) return 0;
        return 1 + std::max(height(node->left), height(node->right));
    }

    // 前序遍历（根-左-右）
    void preorder(std::vector<T>& result) const {
        result.clear();
        preorder(root, result);
    }

    void preorder(BinNode<T>* node, std::vector<T>& result) const {
        if (!node) return;
        result.push_back(node->data);
        preorder(node->left, result);
        preorder(node->right, result);
    }

    // 中序遍历（左-根-右）
    void inorder(std::vector<T>& result) const {
        result.clear();
        inorder(root, result);
    }

    void inorder(BinNode<T>* node, std::vector<T>& result) const {
        if (!node) return;
        inorder(node->left, result);
        result.push_back(node->data);
        inorder(node->right, result);
    }

    // 后序遍历（左-右-根）
    void postorder(std::vector<T>& result) const {
        result.clear();
        postorder(root, result);
    }

    void postorder(BinNode<T>* node, std::vector<T>& result) const {
        if (!node) return;
        postorder(node->left, result);
        postorder(node->right, result);
        result.push_back(node->data);
    }

    // 层序遍历（广度优先）
    void levelorder(std::vector<T>& result) const {
        result.clear();
        if (!root) return;

        std::queue<BinNode<T>*> q;
        q.push(root);

        while (!q.empty()) {
            BinNode<T>* current = q.front();
            q.pop();
            result.push_back(current->data);

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
    }

    // 非递归前序遍历
    void preorderIterative(std::vector<T>& result) const {
        result.clear();
        if (!root) return;

        std::stack<BinNode<T>*> s;
        s.push(root);

        while (!s.empty()) {
            BinNode<T>* current = s.top();
            s.pop();
            result.push_back(current->data);

            if (current->right) s.push(current->right);
            if (current->left) s.push(current->left);
        }
    }

    // 非递归中序遍历
    void inorderIterative(std::vector<T>& result) const {
        result.clear();
        if (!root) return;

        std::stack<BinNode<T>*> s;
        BinNode<T>* current = root;

        while (!s.empty() || current) {
            while (current) {
                s.push(current);
                current = current->left;
            }
            current = s.top();
            s.pop();
            result.push_back(current->data);
            current = current->right;
        }
    }

    // 非递归后序遍历
    void postorderIterative(std::vector<T>& result) const {
        result.clear();
        if (!root) return;

        std::stack<BinNode<T>*> s1, s2;
        s1.push(root);

        while (!s1.empty()) {
            BinNode<T>* current = s1.top();
            s1.pop();
            s2.push(current);

            if (current->left) s1.push(current->left);
            if (current->right) s1.push(current->right);
        }

        while (!s2.empty()) {
            result.push_back(s2.top()->data);
            s2.pop();
        }
    }

    // 获取叶子节点数量
    int countLeaves() const {
        return countLeaves(root);
    }

    int countLeaves(BinNode<T>* node) const {
        if (!node) return 0;
        if (node->isLeaf()) return 1;
        return countLeaves(node->left) + countLeaves(node->right);
    }

    // 插入节点作为左子节点
    bool insertLeft(BinNode<T>* parent, const T& value) {
        if (!parent) return false;
        
        if (parent->left) return false; // 左子节点已存在
        
        parent->left = new BinNode<T>(value, nullptr, nullptr, parent);
        _size++;
        return true;
    }

    // 插入节点作为右子节点
    bool insertRight(BinNode<T>* parent, const T& value) {
        if (!parent) return false;
        
        if (parent->right) return false; // 右子节点已存在
        
        parent->right = new BinNode<T>(value, nullptr, nullptr, parent);
        _size++;
        return true;
    }

    // 删除节点及其子树
    bool remove(BinNode<T>* node) {
        if (!node || !root) return false;
        
        // 如果删除的是根节点
        if (node == root) {
            releaseSubtree(root);
            root = nullptr;
            _size = 0;
            return true;
        }

        // 确定在父节点中删除的是哪个子节点
        BinNode<T>* parent = node->parent;
        if (node == parent->left) {
            releaseSubtree(parent->left);
            parent->left = nullptr;
        } else if (node == parent->right) {
            releaseSubtree(parent->right);
            parent->right = nullptr;
        }
        
        _size -= countNodes(node); // 更新节点数量
        return true;
    }

private:
    // 计算以指定节点为根的子树中的节点数
    int countNodes(BinNode<T>* node) const {
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }
};

#endif // BTREE_H