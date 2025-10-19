#include "Staff.h"
#include "../DAO/StaffDAO.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Staff member definitions
void Staff::display() const {
    cout << left << setw(8) << ID
         << setw(20) << Name
         << setw(25) << Position
         << setw(20) << Phone << endl;
}

void Staff::PrintNV() {
    display();
}

string Staff::getIDnv() { return ID; }
string Staff::getNamenv() { return Name; }
string Staff::getPosition() { return Position; }
void Staff::setPosition(string newPosition) { this->Position = newPosition; }


void printStaffList(StaffDAO &staffDAO) {
    MyVector<Staff*>& sta = staffDAO.getDataCache();
    cout << left << setw(8) << "ID"
         << setw(20) << "Name"
         << setw(25) << "Position"
         << setw(20) << "Phone number" << endl;
    cout << string(73, '-') << endl;
    for (int i = 0; i < sta.getSize(); i++) {
        sta[i]->PrintNV();
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
        string ID_NV, Name_NV, Position, SDT;
        cout << "Enter ID: "; getline(cin, ID_NV);
        if (isduplicateStaff(staffDAO, ID_NV)) {
            cout << "The staff ID already exists. Please enter again." << endl;
            --i;
            continue;
        }
        cout << "Enter name: "; getline(cin, Name_NV);
        cout << "Enter position: "; getline(cin, Position);
        cout << "Enter phone number: "; getline(cin, SDT);

        Staff *nv = new Staff(ID_NV, Name_NV, Position, SDT);
        if (staffDAO.create(ID_NV, nv)) {
            cout << "Staff added successfully." << endl;
        } else {
            cout << "Failed to add staff." << endl;
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
