#include "Customer.h"
#include "../DAO/CustomerDAO.h"
// Customer member definitions 
using namespace std; 
ostream& Customer::output(ostream& os) const {
    os << left << setw(8) << ID
       << setw(20) << Name
       << setw(20) << Phone
       << setw(15) << Address
       << setw(25) << Email;
    return os;
}

istream& Customer::input(istream& is) {
    cout << "Enter ID: ";
    getline(is, ID);
    cout << "Enter name: ";
    getline(is, Name);
    cout << "Enter phone number: ";
    getline(is, Phone);
    cout << "Enter address: ";
    getline(is, Address);
    cout << "Enter email: ";
    getline(is, Email);
    return is;
}

string Customer::getAddress() { return Address; }
string Customer::getEmail() { return Email; }
void Customer::setEmail(string newEmail) { this->Email = newEmail; }
void Customer::setAddress(string newAddress) { this->Address = newAddress; }

// Free functions implementations moved from header
void printCustomerList(CustomerDAO &customerDAO) {
    MyVector<Customer*>& cus = customerDAO.getDataCache();
    if (cus.Empty()) {
        cout << "Customer list is empty." << endl;
        return;
    }
    cout << left << setw(8) << "ID"
         << setw(20) << "Name"
         << setw(20) << "Phone number"
         << setw(15) << "Address"
         << setw(25) << "Email" << endl;
    cout << string(83, '-') << endl;
    for (int i = 0; i < cus.getSize(); i++) {
        cout << *cus[i];
        cout << endl;
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
        Customer *kh = new Customer();
        cin >> *kh;
        if (isduplicateCustomer(customerDAO, kh->getID())) {
            cout << "The customer ID already exists. Please enter again." << endl;
            delete kh; // Cleanup
            --i;
            continue;
        }
        
        if (customerDAO.create(kh->getID(), kh)) {
            cout << "Customer added successfully." << endl;
        } else {
            cout << "Failed to add customer." << endl;
            delete kh;  // Cleanup nếu fail
            --i;
        }
    }
}

void CustomerEdit(Customer* currentUser, CustomerDAO &customerDAO) {
    cout << "---Current personal information---" << endl;
    cout << *currentUser << endl;

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
        cout << *currentUser << endl;
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
                cout << left << setw(8) << "ID"
                     << setw(20) << "Name"
                     << setw(20) << "Phone number"
                     << setw(15) << "Address"
                     << setw(25) << "Email" << endl;
                cout << string(83, '-') << endl;
                found = true;
            }
            cout << *cus[i] << endl;
        }
    }

    if (!found) {
        cout << "Not found!" << endl;
    }
}
