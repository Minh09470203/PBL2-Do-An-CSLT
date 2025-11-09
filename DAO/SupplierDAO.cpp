#include "SupplierDAO.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// callbacks helpers 
static ofstream* globleOutputFile = nullptr;
static void write_supplier_to_file(const string& k, Supplier* const& s) {
    if (!globleOutputFile) return;
    *globleOutputFile << s->getSupID() << "|" << s->getSupName() << "|" << s->getAddress() << "|" << s->getEmail() << "\n";
}
static void delete_supplier_cb(const string& k, Supplier* const& s) {
    if (s) delete s;
}
static void print_supplier_cb(const string& k, Supplier* const& s) {
    if (s) cout << *s << endl;
}

SupplierDAO::~SupplierDAO() {
    dataCache.ForEach(delete_supplier_cb);
    dataCache.Clear();
}

bool SupplierDAO::create(const string &id, Supplier* entity) {
    if (dataCache.Find(id)) {
        cerr << "Supplier ID already exists: " << id << endl;
        return false;
    }
    dataCache.Insert(id, entity);
    ofstream file(filename, ios::app);
    if (!file.is_open()) return false;
    file << entity->getSupID() << "|"
         << entity->getSupName() << "|"
         << entity->getAddress() << "|"
         << entity->getEmail() << "\n";
    file.close();
    return true;
}

Supplier* SupplierDAO::read(const string &id) {
    Supplier** p = dataCache.Find(id);
    return p ? *p : nullptr;
}

bool SupplierDAO::update(const string &id, Supplier* entity) {
    Supplier** existingPtr = dataCache.Find(id);
    if (!existingPtr || !*existingPtr) return false;
    delete *existingPtr;
    dataCache[id] = entity;
    return saveData();
}

bool SupplierDAO::remove(const string &id) {
    Supplier** existingPtr = dataCache.Find(id);
    if (!existingPtr || !*existingPtr) return false;
    delete *existingPtr;
    if (dataCache.Remove(id)) {
        return saveData();
    }
    return false;
}

bool SupplierDAO::loadData() {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        return false;
    }
    // clear existing
    dataCache.ForEach(delete_supplier_cb);
    dataCache.Clear();

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, address, email;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, address, '|');
        getline(ss, email);
        if (!email.empty() && email.back() == '\r') email.pop_back();
        Supplier* s = new Supplier(id, name, address, email);
        dataCache.Insert(id, s);
    }
    file.close();
    return true;
}

bool SupplierDAO::saveData() {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot write to: " << filename << endl;
        return false;
    }
    // iterate and write
    globleOutputFile = &file;
    dataCache.ForEach(write_supplier_to_file);
    globleOutputFile = nullptr;
    file.close();
    return true;
}


void SupplierDAO::displayAll() {
    cout << left << setw(10) << "SupID" << setw(20) << "Supplier Name" << setw(25) << "Address" << setw(25) << "Email" << endl;
    cout << string(80, '-') << endl;
    dataCache.ForEach(print_supplier_cb);
}