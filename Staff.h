#ifndef STAFF_H_INCLUDED
#define STAFF_H_INCLUDED
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;
class Staff {
private:
    string ID_NV;
    string Name_NV;
    string Position;
    string SDT;
public:
    Staff(
          string ID_NV = "",
          string Name_NV = "",
          string Position = "",
          string SDT = ""
    )
    : ID_NV(ID_NV), Name_NV(Name_NV), Position(Position), SDT(SDT) {}

    void PrintNV() {
        cout << left << setw(8) << ID_NV
            << setw(20) << Name_NV
            << setw(25) << Position
            << setw(20) << SDT << endl;
    }

    string getIDnv() { return ID_NV; }
    string getNamenv() { return Name_NV; }
    string getPosition() { return Position; }
    string getPhone() { return SDT; }
};

void printStaffList(MyVector<Staff*> &sta) {
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

// StaffInsert function
void StaffInsert(MyVector<Staff*> &sta, int n) {
    ofstream file("Data/Staff.txt", ios::app);
    if (!file.is_open()) {
        cout << "Cannot open file Staff.txt" << endl;
        return;
    }

    for(int i = 0; i < n; i++) {
        string ID_NV, Name_NV, Position, SDT;
        cout << "Enter ID: "; getline(cin, ID_NV);
        cout << "Enter name: "; getline(cin, Name_NV);
        cout << "Enter position: "; getline(cin, Position);
        cout << "Enter phone number: "; getline(cin, SDT);

        Staff *nv = new Staff(ID_NV, Name_NV, Position, SDT);
        sta.Push_back(nv);

        // Write each staff to file immediately
        file << ID_NV << "|" << Name_NV << "|" << Position << "|" << SDT << "\n";
    }
    file.close();
    cout << "Staff added successfully." << endl;
}

//StaffDelete function
void StaffDelete(MyVector<Staff*> &sta) {
    if (sta.Empty()) {
        cout << "The staff list is empty." << endl;
        return;
    }

    string ma;
    cout << "Enter the staff ID to delete: ";
    getline(cin, ma);

    bool found = false;
    for (int i = 0; i < sta.getSize(); i++) {
        if (sta[i]->getIDnv() == ma) {
            found = true;
            delete sta[i];

            // Shift elements forward and decrease size
            for (int j = i; j < sta.getSize() - 1; j++) {
                sta[j] = sta[j + 1];
            }
            // Manually decrease size since MyVector doesn't have remove method
            sta.Clear();

            cout << "Staff with ID " << ma << " has been deleted successfully." << endl;
            break;
        }
    }

    if (!found) {
        cout << "Staff with ID " << ma << " not found." << endl;
        return;
    }

    // Rewrite entire Staff.txt file
    ofstream file("Data/Staff.txt");
    if (!file.is_open()) {
        cout << "Cannot open Staff.txt" << endl;
        return;
    }

    for (int i = 0; i < sta.getSize(); i++) {
        file << sta[i]->getIDnv() << "|"
             << sta[i]->getNamenv() << "|"
             << sta[i]->getPosition() << "|"
             << sta[i]->getPhone() << "\n";
    }
    file.close();
}

bool loadStaffFile(const string& filename, MyVector<Staff*> &sta) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, position, phone;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, position, '|');
        getline(ss, phone);
        Staff *s = new Staff(id, name, position, phone);
        sta.Push_back(s);
    }
    file.close();
    return true;
}
#endif // STAFF_H_INCLUDED
