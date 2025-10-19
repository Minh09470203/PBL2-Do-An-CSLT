#include "Supplier.h"
#include "../DAO/SupplierDAO.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

// Supplier members
string Supplier::getSupID() { return ID_Supplier; }
string Supplier::getSupName() { return Name_Supplier; }
void Supplier::setSupID(string SupID) { this->ID_Supplier = SupID; }
void Supplier::setSupName(string SupName) { this->Name_Supplier = SupName; }


// Free functions
void AddSupplier(SupplierDAO &supplierDAO) {
    string id, name;
    cout << "Enter new Supplier ID: ";
    getline(cin, id);
    cout << "Enter Supplier Name: ";
    getline(cin, name);

    if (supplierDAO.read(id) != nullptr) {
        cout << "Supplier ID already exists.\n";
        return;
    }

    if (supplierDAO.addSupplier(id, name)) {
        cout << "Supplier added successfully.\n";
    } else {
        cout << "Failed to add supplier.\n";
    }
}

void ShowSupplierList(SupplierDAO &supplierDAO) {
    supplierDAO.displayAll();
}


