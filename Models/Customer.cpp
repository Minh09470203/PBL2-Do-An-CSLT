#include "Customer.h"
#include "../DAO/CustomerDAO.h"
// Customer member definitions 
void Customer::display() const {
    cout << left << setw(8) << ID
         << setw(20) << Name
         << setw(20) << Phone
         << setw(15) << Address
         << setw(25) << Email << endl;
}

void Customer::PrintKH() {
    display();
}

string Customer::getAddress() { return Address; }
string Customer::getEmail() { return Email; }
void Customer::setEmail(string newEmail) { this->Email = newEmail; }
void Customer::setAddress(string newAddress) { this->Address = newAddress; }

// Free functions implementations moved from header
void printCustomerList(CustomerDAO &customerDAO) {
    MyVector<Customer*>& cus = customerDAO.getDataCache();
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

bool isduplicateCustomer(CustomerDAO &customerDAO, const string& id) {
    if (customerDAO.read(id) != nullptr) {
        return true;
    }
    return false;
}

void CustomerInsert(CustomerDAO &customerDAO) {
    MyVector<Customer*>& cus = customerDAO.getDataCache();
    int n;
    cout << "Enter number of customers to add: ";
    cin >> n;
    cin.ignore(); 
    for(int i = 0; i < n; i++) {
        string ID_KH, Name_KH, SDT, Address, Email;
        cout << "Enter ID: "; getline(cin, ID_KH);
        if (isduplicateCustomer(customerDAO, ID_KH)) {
            cout << "The customer ID already exists. Please enter again." << endl;
            --i;
            continue;
        }
        cout << "Enter name: "; getline(cin, Name_KH);
        cout << "Enter phone number: "; getline(cin, SDT);
        cout << "Enter address: "; getline(cin, Address);
        cout << "Enter email: "; getline(cin, Email);

        Customer *kh = new Customer(ID_KH, Name_KH, SDT, Address, Email);
        if (customerDAO.create(ID_KH, kh)) {
            cout << "Customer added successfully." << endl;
        } else {
            cout << "Failed to add customer." << endl;
            delete kh;  // Cleanup nếu fail
            --i;
        }
    }
    cout << "Customer added successfully." << endl;
}

void CustomerEdit(Customer* currentUser, CustomerDAO &customerDAO) {
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
        MyVector<Customer*>& cus = customerDAO.getDataCache();
        // Rewrite entire Customer.txt file
        customerDAO.saveAll();
        cout << "Your information has been saved." << endl;
    } else {
        cout << "\nUpdate canceled. Information unchanged." << endl;
    }
}

void CustomerSearch(CustomerDAO &customerDAO) {
    MyVector<Customer*>& cus = customerDAO.getDataCache();
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
                cout << "Search results:" << endl;
                cout << left << setw(8) << "ID"
                     << setw(20) << "Name"
                     << setw(20) << "Phone number"
                     << setw(15) << "Address"
                     << setw(25) << "Email" << endl;
                cout << string(83, '-') << endl;
                found = true;
            }
            cus[i]->PrintKH();
        }
    }

    if (!found) {
        cout << "Not found!" << endl;
    }
}
