#include "Supplier.h"
#include "../DAO/SupplierDAO.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

// Supplier members
string Supplier::getSupID() const { return ID_Supplier; }
string Supplier::getSupName() const { return Name_Supplier; }
void Supplier::setSupID(string SupID) { this->ID_Supplier = SupID; }
void Supplier::setSupName(string SupName) { this->Name_Supplier = SupName; }
string Supplier::getEmail() const { return Email; }
string Supplier::getPhoneNumber() const { return phoneNumber; }
void Supplier::setEmail(string email) { this->Email = email; }
void Supplier::setPhoneNumber(string phone) { this->phoneNumber = phone; }

// Return a human-friendly formatted phone number without altering stored value.
string Supplier::getFormattedPhone() const {
    string d;
    d.reserve(phoneNumber.size());
    for (char c : phoneNumber) if (std::isdigit((unsigned char)c)) d.push_back(c);
    // Common formatting rules:
    // - 10 digits: 4-3-3 (e.g. 0903-123-456) as requested
        if (d.size() == 10 && d[0] == '0') {
            // Format exactly as +84-903-123-456 for local numbers like 0903123456
            string rest = d.substr(1); // 9 digits
            string formatted = rest.substr(0,3) + "-" + rest.substr(3,3) + "-" + rest.substr(6,3);
            return string("+84-") + formatted;
        }
        // For any other input, return digits-only (no other formatting)
        return d;
}

// Helper: validate phone (digits-only) must be 10 digits and start with '0'
static bool isValidPhone(const string &digits) {
    if (digits.size() != 10) return false;
    if (digits[0] != '0') return false;
    for (char c : digits) if (!std::isdigit((unsigned char)c)) return false;
    return true;
}

// Free helper implementation (matches declaration in Supplier.h)
string formatNumber(unsigned long v) {
    string s = to_string(v);
    int n = (int)s.length();
    int insertPos = n - 3;
    while (insertPos > 0) {
        s.insert((size_t)insertPos, ",");
        insertPos -= 3;
    }
    return s;
}


// Free functions
void AddSupplier(SupplierDAO &supplierDAO) {
    Supplier newSupplier;
    cin >> newSupplier;

    if (supplierDAO.read(newSupplier.getSupID()) != nullptr) {
        cout << "Supplier ID already exists.\n";
        return;
    }

    // allocate and give ownership to DAO
    // constructor order: ID, Name, Email, Phone (phone stored normalized)
    Supplier* p = new Supplier(newSupplier.getSupID(), newSupplier.getSupName(), newSupplier.getEmail(), newSupplier.getPhoneNumber());
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
    // Print: ID | Name | Email | Phone (phone as last column, formatted)
    os << left << setw(10) << ID_Supplier << setw(20) << Name_Supplier << setw(25) << Email << setw(20) << getFormattedPhone();
    return os;
}

ostream& operator<<(ostream& os, const Supplier& s) {
    return s.output(os);
}

istream& Supplier::input(istream& is) {
    cout << "Enter supplier ID: "; getline(is, ID_Supplier);
    cout << "Enter supplier name: "; getline(is, Name_Supplier);
    cout << "Enter email: "; getline(is, Email);
    // Prompt until valid phone is entered (10 digits, starting with 0)
    while (true) {
        cout << "Enter phone number (10 digits, starts with 0): "; string rawPhone; getline(is, rawPhone);
        // Normalize: keep digits only
        string digits;
        digits.reserve(rawPhone.size());
        for (char c : rawPhone) if (std::isdigit((unsigned char)c)) digits.push_back(c);
        if (!isValidPhone(digits)) {
            cout << "Invalid phone number. It must contain exactly 10 digits and start with '0'. Please try again." << endl;
            continue;
        }
        phoneNumber = digits;
        break;
    }
    return is;
}

istream& operator>>(istream& is, Supplier& s) {
    return s.input(is);
}

