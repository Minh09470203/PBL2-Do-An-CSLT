#include "SupplierDAO.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool SupplierDAO::addSupplier(const string& id, const string& name) {
    string n = name;
    return create(id, &n);
}

bool SupplierDAO::updateSupplier(const string& id, const string& newName) {
    string n = newName;
    return update(id, &n);
}

bool SupplierDAO::deleteSupplier(const string& id) {
    return remove(id);
}

bool SupplierDAO::supplierExists(const string& id) {
    return read(id) != nullptr;
}

void SupplierDAO::displayAll() {
    cout << left << setw(10) << "SupID " << setw(20) << " Supplier Name" << endl;
    cout << string(25, '-') << endl;
    dataCache.ShowAll();
}