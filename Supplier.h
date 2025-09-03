#ifndef SUPPLIER_H_INCLUDED
#define SUPPLIER_H_INCLUDED
#include <fstream>

// Data structure map
template<typename K, typename V>
class MyMap {
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
    };

    Node* root;

    // Insert node function
    void Insert(Node*& node, K key, V value) {
        if (!node) {
            node = new Node{key, value, nullptr, nullptr};
        } else if (key < node->key) {
            Insert(node->left, key, value);
        } else if (key > node->key) {
            Insert(node->right, key, value);
        } else {
            node->value = value; // Update if existed
        }
    }

    // Seatch node function
    V* Find(Node* node, K key) {
        if (!node) return nullptr;
        if (key == node->key) return &node->value;
        if (key < node->key) return Find(node->left, key);
        return Find(node->right, key);
    }

    // Free node
    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    MyMap() : root(nullptr) {}

    ~MyMap() {
        destroy(root);
    }

    void Insert(K key, V value) {
        Insert(root, key, value);
    }

    V* Find(K key) {
        return Find(root, key);
    }

    // operator[] like std::map
    V& operator[](K key) {
        V* val = Find(root, key);
        if (val) {
            return *val;
        } else {
            Insert(root, key, V()); // Setup value
            return *Find(root, key);
        }
    }
    void Traverse(Node* node) {
        if (!node) return;
        Traverse(node->left);
        cout << setw(10) << node->key << "|  " << setw(20) << node->value << endl;
        Traverse(node->right);
    }

    void ShowAll() {
        Traverse(root);
    }

};

class Supplier {
    private:
        string ID_Supplier;
        string Name_Supplier;
        string Address;
        string SDT;
    public:
        Supplier(
                 string ID_Supplier = "",
                 string Name_Supplier = "",
                 string Address = "",
                 string SDT = ""
        )
        : ID_Supplier(ID_Supplier), Name_Supplier(Name_Supplier), Address(Address), SDT(SDT) {}
        string getSupID() {
            return ID_Supplier;
        }
        string getSupName() {
            return Name_Supplier;
        }
        void setSupID(string SupID) {
            this->ID_Supplier = SupID;
        }
        void setSupName(string SupName) {
            this->Name_Supplier = SupName;
        }
};

MyMap<string, string> supplierMap;

bool loadSupplierMap(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file " << filename << endl;
        return false;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name;
        getline(ss, id, '|');
        getline(ss, name);
        supplierMap[id] = name;
    }
    file.close();
    return true;
}

void AddSupplier(MyMap<string, string> supplierMap) {
    string id, name;
    cout << "Enter new Supplier ID: ";
    getline(cin, id);
    cout << "Enter Supplier Name: ";
    getline(cin, name);

    // Check duplicate
    if (supplierMap.Find(id)) {
        cout << "Supplier ID already exists.\n";
        return;
    }

    supplierMap.Insert(id, name);

    ofstream file("Data/Supplier.txt", ios::app);
    if (file.is_open()) {
        file << id << "|" << name << "\n";
        file.close();
        cout << "Supplier added successfully.\n";
    } else {
        cout << "Cannot open Supplier.txt.";
    }
}

void ShowSupplierList(MyMap<string, string> supplierMap) {
    cout << left << setw(10) << "SupID "
         << setw(20) << " Supplier Name" << endl;
    cout << string(25, '-') << endl;
    supplierMap.ShowAll();
}

#endif // SUPPLIER_H_INCLUDED
