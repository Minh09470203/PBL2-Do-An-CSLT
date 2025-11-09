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
string Supplier::getAddress() { return Address; }
string Supplier::getEmail() { return Email; }
void Supplier::setAddress(string address) { this->Address = address; }
void Supplier::setEmail(string email) { this->Email = email; }


// Free functions
void AddSupplier(SupplierDAO &supplierDAO) {
    Supplier newSupplier;
    cin >> newSupplier;

    if (supplierDAO.read(newSupplier.getSupID()) != nullptr) {
        cout << "Supplier ID already exists.\n";
        return;
    }

    // allocate and give ownership to DAO
    Supplier* p = new Supplier(newSupplier.getSupID(), newSupplier.getSupName(), newSupplier.getAddress(), newSupplier.getEmail());
    if (supplierDAO.create(p->getSupID(), p)) {
        cout << "Supplier added successfully.\n";
    } else {
        delete p;
        cout << "Failed to add supplier.\n";
    }
}

void ShowSupplierList(SupplierDAO &supplierDAO) {
    supplierDAO.displayAll();
}

// Supplier stream I/O
ostream& Supplier::output(ostream& os) const {
    os << left << setw(10) << ID_Supplier << setw(20) << Name_Supplier << setw(25) << Address << setw(25) << Email;
    return os;
}

ostream& operator<<(ostream& os, const Supplier& s) {
    return s.output(os);
}

istream& Supplier::input(istream& is) {
    cout << "Enter supplier ID: "; getline(is, ID_Supplier);
    cout << "Enter supplier name: "; getline(is, Name_Supplier);
    cout << "Enter address: "; getline(is, Address);
    cout << "Enter email: "; getline(is, Email);
    return is;
}

istream& operator>>(istream& is, Supplier& s) {
    return s.input(is);
}

