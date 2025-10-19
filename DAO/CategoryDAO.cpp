#include "CategoryDAO.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

string CategoryDAO::getCategoryName(const string& id) {
    string* name = read(id);
    return name ? *name : "";
}

bool CategoryDAO::addCategory(const string& id, const string& name) {
    string n = name;
    return create(id, &n);
}

bool CategoryDAO::updateCategory(const string& id, const string& newName) {
    string n = newName;
    return update(id, &n);
}

bool CategoryDAO::deleteCategory(const string& id) {
    return remove(id);
}

bool CategoryDAO::categoryExists(const string& id) {
    return read(id) != nullptr;
}

void CategoryDAO::displayAll() {
    cout << left << setw(10) << "CateID " << setw(20) << " Category Name" << endl;
    cout << string(25, '-') << endl;
    dataCache.ShowAll();
}