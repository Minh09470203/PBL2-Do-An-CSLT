#include "Staff.h"
#include "../DAO/StaffDAO.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Staff member definitions
ostream& Staff::output(ostream& os) const {
    os << left << setw(8) << ID
       << setw(20) << Name
       << setw(25) << Position
       << setw(20) << Phone;
    return os;
}

istream& Staff::input(istream& is) {
    cout << "Enter ID: ";
    getline(is, ID);
    cout << "Enter name: ";
    getline(is, Name);
    cout << "Enter position: ";
    getline(is, Position);
    cout << "Enter phone number: ";
    getline(is, Phone);
    return is;
}

string Staff::getIDnv() { return ID; }
string Staff::getNamenv() { return Name; }
string Staff::getPosition() { return Position; }
void Staff::setPosition(string newPosition) { this->Position = newPosition; }


void printStaffList(StaffDAO &staffDAO) {
    MyVector<Staff*>& sta = staffDAO.getDataCache();
    if (sta.Empty()) {
        cout << "Staff list is empty." << endl;
        return;
    }
    cout << left << setw(8) << "ID"
         << setw(20) << "Name"
         << setw(25) << "Position"
         << setw(20) << "Phone number" << endl;
    cout << string(73, '-') << endl;
    for (int i = 0; i < sta.getSize(); i++) {
        cout << *sta[i] << endl;
    }
    cout << string(73, '-') << endl;
}

bool isduplicateStaff(StaffDAO &staffDAO, const string& id) {
    if (staffDAO.read(id) != nullptr) {
        return true;
    }
    return false;
}

void StaffInsert(StaffDAO &staffDAO) {
    MyVector<Staff*>& sta = staffDAO.getDataCache();

    int n;
    cout << "Enter number of staff to add: ";
    cin >> n;
    cin.ignore();  
    for(int i = 0; i < n; i++) {
        Staff *nv = new Staff();
        cin >> *nv;
        if (isduplicateStaff(staffDAO, nv->getIDnv())) {
            cout << "The staff ID already exists. Please enter again." << endl;
            delete nv; // Cleanup
            --i;
            continue;
        }

        if (staffDAO.create(nv->getIDnv(), nv)) {
            cout << "Staff added successfully." << endl;
        } else {
            cout << "Failed to add staff." << endl;
            delete nv;  // Cleanup nếu fail
            --i;
        }
    }
}

void StaffDelete(StaffDAO &staffDAO) {
    MyVector<Staff*>& sta = staffDAO.getDataCache();
    if (sta.Empty()) {
        cout << "The staff list is empty." << endl;
        return;
    }

    string ma;
    cout << "Enter the staff ID to delete: ";
    getline(cin, ma);

    if (staffDAO.remove(ma)) {
        cout << "Staff with ID " << ma << " has been deleted successfully." << endl;
    } else {
        cout << "Staff with ID " << ma << " not found." << endl;
    }
}
