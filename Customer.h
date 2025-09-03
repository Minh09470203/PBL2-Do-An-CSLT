#ifndef CUSTOMER_H_INCLUDED
#define CUSTOMER_H_INCLUDED
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

// Data structure vector
template<typename T>
class MyVector {
private:
    T* data;
    int capacity;
    int Size;

    void Resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (int i = 0; i < Size; ++i)
            newData[i] = data[i];
        delete[] data;
        data = newData;
    }

public:
    MyVector() : capacity(10), Size(0) {
        data = new T[capacity];
    }

    void Push_back(const T& value) {
        if (Size == capacity)
            Resize();
        data[Size++] = value;
    }

    T& operator[](int index) {
        if (index < 0 || index >= Size)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    int getSize() const {
        return Size;
    }

    bool Empty() const {
        return Size == 0;
    }

    void Clear() {
        Size = 0;
    }

    ~MyVector() {
        delete[] data;
    }
};

class Customer {
private:
    string ID_KH;
    string Name_KH;
    string SDT;
    string Address;
    string Email;
public:
    Customer(
             string ID_KH = "",
             string Name_KH = "",
             string SDT = "",
             string Address = "",
             string Email = ""
    )
    : ID_KH(ID_KH), Name_KH(Name_KH), SDT(SDT), Address(Address), Email(Email) {}

    void PrintKH() {
        cout << left << setw(8) << ID_KH
             << setw(20) << Name_KH
             << setw(20) << SDT
             << setw(15) << Address
             << setw(25) << Email << endl;
    }

    string getID() { return ID_KH; }
    string getName() { return Name_KH; }
    void setName(string newName) { this->Name_KH = newName; }
    string getPhone() { return SDT; }
    void setPhone(string newPhone) { this->SDT = newPhone; }
    string getAddress() { return Address; }
    string getEmail() { return Email; }
    void setEmail(string newEmail) { this->Email = newEmail; }
};

void printCustomerList(MyVector<Customer*> &cus) {
    cout << left << setw(8) << "ID"
         << setw(20) << "Name"
         << setw(20) << "Phone number"
         << setw(15) << "Address"
         << setw(25) << "Email" << endl;
    cout << string(83, '-') << endl;
    for (int i = 0; i < cus.getSize(); i++) {
        cus[i]->PrintKH();
    }
    cout << string(83, '-') << endl;
}

// FIXED: CustomerInsert function
void CustomerInsert(MyVector<Customer*> &cus, int n) {
    ofstream file("Data/Customer.txt", ios::app);
    if (!file.is_open()) {
        cout << "Cannot open file Customer.txt" << endl;
        return;
    }

    for(int i = 0; i < n; i++) {
        string ID_KH, Name_KH, SDT, Address, Email;
        cout << "Enter ID: "; getline(cin, ID_KH);
        cout << "Enter name: "; getline(cin, Name_KH);
        cout << "Enter phone number: "; getline(cin, SDT);
        cout << "Enter address: "; getline(cin, Address);
        cout << "Enter email: "; getline(cin, Email);

        Customer *kh = new Customer(ID_KH, Name_KH, SDT, Address, Email);
        cus.Push_back(kh);

        // Write each customer to file immediately
        file << ID_KH << "|" << Name_KH << "|" << SDT << "|" << Address << "|" << Email << "\n";
    }
    file.close();
    cout << "Customer added successfully." << endl;
}

// FIXED: CustomerEdit function
void CustomerEdit(Customer* currentUser, MyVector<Customer*> &cus) {
    cout << "---Current personal information---" << endl;
    currentUser->PrintKH();

    string newName, newPhone, newEmail;
    cout << "Enter new name (or press Enter to keep current): ";
    getline(cin, newName);
    cout << "Enter new phone number (or press Enter to keep current): ";
    getline(cin, newPhone);
    cout << "Enter new email (or press Enter to keep current): ";
    getline(cin, newEmail);

    cout << "\nUpdated Information:" << endl;
    cout << "New name: " << (!newName.empty() ? newName : currentUser->getName()) << endl;
    cout << "New phone number: " << (!newPhone.empty() ? newPhone : currentUser->getPhone()) << endl;
    cout << "New email: " << (!newEmail.empty() ? newEmail : currentUser->getEmail()) << endl;

    string confirm;
    cout << "\nDo you want to update your information? (y/n): ";
    do {
        getline(cin, confirm);
        if (confirm == "y" || confirm == "Y" || confirm == "n" || confirm == "N") {
            break;
        }
        cout << "Please only choose Yes or No (y/n): ";
    } while (true);

    if (confirm == "y" || confirm == "Y") {
        if (!newName.empty()) currentUser->setName(newName);
        if (!newPhone.empty()) currentUser->setPhone(newPhone);
        if (!newEmail.empty()) currentUser->setEmail(newEmail);

        cout << "\nUpdate successful." << endl;
        currentUser->PrintKH();

        // Rewrite entire Customer.txt file
        ofstream file("Data/Customer.txt");
        if (!file.is_open()) {
            cout << "Cannot open Customer.txt" << endl;
            return;
        }

        for (int i = 0; i < cus.getSize(); ++i) {
            file << cus[i]->getID() << "|"
                 << cus[i]->getName() << "|"
                 << cus[i]->getPhone() << "|"
                 << cus[i]->getAddress() << "|"
                 << cus[i]->getEmail() << "\n";
        }
        file.close();
    } else {
        cout << "\nUpdate canceled. Information unchanged." << endl;
    }
}

// FIXED: CustomerSearch function
void CustomerSearch(MyVector<Customer*> &cus) {
    if (cus.Empty()) {
        cout << "Customer list is empty." << endl;
        return;
    }

    string keyword;
    cout << "Enter customer name or phone number: ";
    getline(cin, keyword);

    bool found = false;
    for(int i = 0; i < cus.getSize(); i++) {
        if (cus[i]->getName() == keyword || cus[i]->getPhone() == keyword) {
            if (!found) {
                cout << "Search results:" << endl;
                printCustomerList(cus); // Print header
                found = true;
            }
            cus[i]->PrintKH();
        }
    }

    if (!found) {
        cout << "Not found!" << endl;
    }
}

bool loadCustomerFile(const string& filename, MyVector<Customer*> &cus) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, phone, address, email;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, phone, '|');
        getline(ss, address, '|');
        getline(ss, email);
        Customer *c = new Customer(id, name, phone, address, email);
        cus.Push_back(c);
    }
    file.close();
    return true;
}
#endif // CUSTOMER_H_INCLUDED
