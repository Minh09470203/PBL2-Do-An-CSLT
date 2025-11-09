#include "CategoryDAO.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// callbacks helpers 
static ofstream* globleOutputFile = nullptr;
static void write_category_to_file(const string& k, Category* const& c) {
    if (!globleOutputFile || !c) return;
    *globleOutputFile << c->getID_Category() << "|" << c->getName_Category() << "\n";
}

static void delete_category_cb(const string& k, Category* const& c) { 
    if (c) delete c; 
}

static void print_category_cb(const string& k, Category* const& c) { 
    if (c) cout << *c << endl; 
}

CategoryDAO::~CategoryDAO() {
    getDataCache().ForEach(delete_category_cb);
    getDataCache().Clear();
}

bool CategoryDAO::create(const string& id, Category* entity) {
    if (getDataCache().Find(id)) {
        cerr << "ID already exists: " << id << endl;
        return false;
    }
    getDataCache().Insert(id, entity);
    ofstream file(filename, ios::app);
    if (!file.is_open()) return false;
    file << entity->getID_Category() << "|" << entity->getName_Category() << "\n";
    file.close();
    return true;
}

    
Category* CategoryDAO::read(const string& id) {
    Category** p = getDataCache().Find(id);
    return p ? *p : nullptr;
}

bool CategoryDAO::update(const string& id, Category* entity) {
    Category** p = getDataCache().Find(id);
    if (!p || !*p) return false;
    delete *p;
    getDataCache()[id] = entity;
    return saveData();
}

bool CategoryDAO::remove(const string& id) {
    Category** p = getDataCache().Find(id);
    if (!p || !*p) return false;
    delete *p;
    if (getDataCache().Remove(id)) return saveData();
    return false;
}

bool CategoryDAO::loadData() {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        return false;
    }
    // clear existing
    getDataCache().ForEach(delete_category_cb);
    getDataCache().Clear();

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name;
        getline(ss, id, '|');
        getline(ss, name);
        if (!id.empty() && id.back() == '\r') id.pop_back();
        if (!name.empty() && name.back() == '\r') name.pop_back();
        Category* c = new Category(id, name);
        getDataCache().Insert(id, c);
    }
    file.close();
    return true;
}

bool CategoryDAO::saveData() {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot write to: " << filename << endl;
        return false;
    }
    globleOutputFile = &file;
    getDataCache().ForEach(write_category_to_file);
    globleOutputFile = nullptr;
    file.close();
    return true;
}

void CategoryDAO::displayAll() {
    cout << left << setw(10) << "CateID" << setw(30) << "Category Name" << endl;
    cout << string(23, '-') << endl;
    getDataCache().ForEach(print_category_cb);
}