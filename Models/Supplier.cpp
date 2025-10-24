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
    Supplier newSupplier;
    cin >> newSupplier;

    if (supplierDAO.read(newSupplier.getSupID()) != nullptr) {
        cout << "Supplier ID already exists.\n";
        return;
    }

    if (supplierDAO.addSupplier(newSupplier.getSupID(), newSupplier.getSupName())) {
        cout << "Supplier added successfully.\n";
    } else {
        cout << "Failed to add supplier.\n";
    }
}

void ShowSupplierList(SupplierDAO &supplierDAO) {
    supplierDAO.displayAll();
}

// Supplier stream I/O
ostream& Supplier::output(ostream& os) const {
    os << left << setw(10) << ID_Supplier << setw(20) << Name_Supplier << setw(20) << Address << setw(15) << SDT;
    return os;
}

ostream& operator<<(ostream& os, const Supplier& s) {
    return s.output(os);
}

istream& Supplier::input(istream& is) {
    cout << "Enter supplier ID: "; getline(is, ID_Supplier);
    cout << "Enter supplier name: "; getline(is, Name_Supplier);
    return is;
}

istream& operator>>(istream& is, Supplier& s) {
    return s.input(is);
}

