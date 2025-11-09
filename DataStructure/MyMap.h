#ifndef MYMAP_H_INCLUDED
#define MYMAP_H_INCLUDED

// Data structure map
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

template<typename K, typename V>
class MyMap {
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
    };

    Node* root;
    int size;  // Count nodes

    void Insert(Node*& node, K key, V value) {
        if (!node) {
            node = new Node{key, value, nullptr, nullptr};
            size++;  
        } else if (key < node->key) {
            Insert(node->left, key, value);
        } else if (key > node->key) {
            Insert(node->right, key, value);
        } else {
            node->value = value;  // Update
        }
    }

    V* Find(Node* node, K key) {
        if (!node) return nullptr;
        if (key == node->key) return &node->value;
        if (key < node->key) return Find(node->left, key);
        return Find(node->right, key);
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
    
    // Traverse với callback (cho DAO)
    void Traverse(Node* node, void (*callback)(const K&, const V&)) {
        if (!node) return;
        Traverse(node->left, callback);
        callback(node->key, node->value);
        Traverse(node->right, callback);
    }
    
    // Traverse với ofstream (ghi file)
    void TraverseToFile(Node* node, ofstream& file) {
        if (!node) return;
        TraverseToFile(node->left, file);
        file << node->key << "|" << node->value << "\n";
        TraverseToFile(node->right, file);
    }

    // Remove method (xóa node)
    Node* Remove(Node* node, K key, bool& removed) {
        if (!node) {
            removed = false;
            return nullptr;
        }
        
        if (key < node->key) {
            node->left = Remove(node->left, key, removed);
        } else if (key > node->key) {
            node->right = Remove(node->right, key, removed);
        } else {
            // Found the node to delete
            removed = true;
            size--;
            
            // Case 1: No children
            if (!node->left && !node->right) {
                delete node;
                return nullptr;
            }
            // Case 2: One child
            else if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            // Case 3: Two children - find inorder successor
            else {
                Node* successor = node->right;
                while (successor->left) {
                    successor = successor->left;
                }
                node->key = successor->key;
                node->value = successor->value;
                node->right = Remove(node->right, successor->key, removed);
            }
        }
        return node;
    }

public:
    MyMap() : root(nullptr), size(0) {}

    ~MyMap() {
        destroy(root);
    }

    void Insert(K key, V value) {
        Insert(root, key, value);
    }

    V* Find(K key) {
        return Find(root, key);
    }
    
    // Public Remove method
    bool Remove(K key) {
        bool removed = false;
        root = Remove(root, key, removed);
        return removed;
    }

    V& operator[](K key) {
        V* val = Find(root, key);
        if (val) {
            return *val;
        } else {
            Insert(root, key, V());
            return *Find(root, key);
        }
    }
    
    // Traverse display
    void Traverse(Node* node) {
        if (!node) return;  
        Traverse(node->left);
        cout << setw(10) << node->key << "|  "
             << setw(20) << node->value << endl;
        Traverse(node->right);
    }

    void ShowAll() {
        Traverse(root);
    }
    
    
    int Count() const {
        return size;
    }
    
    
    void Clear() {
        destroy(root);
        root = nullptr;
        size = 0;
    }
    
   
    void ForEach(void (*callback)(const K&, const V&)) {
        Traverse(root, callback);
    }
    
    void WriteToFile(ofstream& file) {
        TraverseToFile(root, file);
    }
    
    bool Empty() const {
        return size == 0;
    }
};
#endif // MYMAP_H_INCLUDED