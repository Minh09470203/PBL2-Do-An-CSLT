#include "Account.h"
#include "../AppSession/SessionManager.h"
#include <conio.h>
#include "../DAO/CustomerDAO.h"
#include "../DAO/AccountDAO.h"
#include "Customer.h"

// Read password from console and display '*' for each character (Windows _getch)
static std::string readPasswordMasked() {
    std::string pwd;
    int ch;
    while (true) {
        ch = _getch();
        if (ch == '\r' || ch == '\n') {
            std::cout << std::endl;
            break;
        }
        if (ch == 8 || ch == 127) { // Backspace
            if (!pwd.empty()) {
                pwd.pop_back();
                std::cout << "\b \b";
            }
        } else if (ch == 3) { // Ctrl-C
            // ignore
        } else {
            pwd.push_back((char)ch);
            std::cout << '*';
        }
    }
    return pwd;
}

// Change password function
void ChangePassword(AccountDAO &accountDAO) {
    SessionManager& session = SessionManager::getInstance();
    string currentUsername = session.getCurrentUsername();
    
    string oldPassword, newPassword, confirmPassword;
        cout << "Enter old password: "; oldPassword = readPasswordMasked();
        cout << "Enter new password: "; newPassword = readPasswordMasked();
        cout << "Confirm new password: "; confirmPassword = readPasswordMasked();

    if (newPassword != confirmPassword) {
        cout << "Passwords do not match!\n";
        return;
    }

    Account* account = accountDAO.read(currentUsername);
    if (account && account->getPassword() == oldPassword) {
        account->setPassword(newPassword);  
        accountDAO.update(currentUsername, account); 
        cout << "Password changed successfully!\n";
    } else {
        cout << "Old password is incorrect!\n";
    }
}

// Login function
int Login(CustomerDAO &customerDAO, StaffDAO &staffDAO, AccountDAO &accountDAO) {
    string username, password;
    cout << "\n========== LOGIN ==========\n";
    cout << "Username: "; getline(cin, username);
    cout << "Password: "; password = readPasswordMasked();

    SessionManager& session = SessionManager::getInstance();

    Account* account = accountDAO.read(username);
    
    if (account && account->getPassword() == password) {
        session.setCurrentUsername(username);
        
        if (account->getUserType() == "staff") {
            // Find staff
            Staff* staff = staffDAO.read(account->getUserID());
            if (staff) {
                session.setCurrentStaff(staff);
                cout << "Staff login successful! Welcome " << session.getCurrentStaff()->getNamenv() << "\n";
                return 1; // Staff
            } else {
                cout << "Staff data not found!\n";
                return 0;
            }
        }
        else if (account->getUserType() == "customer") {
            // Find customer
            Customer* customer = customerDAO.read(account->getUserID());
            if (customer) {
                session.setCurrentCustomer(customer);
                cout << "Customer login successful! Welcome " << session.getCurrentCustomer()->getName() << "\n";
                return 2; // Customer
            } else {
                cout << "Customer data not found!\n";
                return 0;
            }
        }
    }

    cout << "Invalid username or password!\n";
    return 0; // Login failed
}

ostream &Account::output(ostream &os) const {
    os << left << setw(15) << username << setw(15) << userType << setw(15) << userID;
    return os;
}
ostream &operator<<(ostream &os, const Account &a) {
    return a.output(os);
}

// Register function: create a new Customer and Account and persist both
void Register(CustomerDAO &customerDAO, AccountDAO &accountDAO) {
    cout << "\n=== REGISTER CUSTOMER ACCOUNT ===\n";
    string username;
    // choose unique username
    while (true) {
        cout << "Choose a username: ";
        getline(cin, username);
        if (username.empty()) { cout << "Username cannot be empty.\n"; continue; }
        if (accountDAO.read(username) != nullptr) {
            cout << "Username already exists. Choose another.\n";
            continue;
        }
        break;
    }

    string password, confirm;
    while (true) {
        cout << "Choose a password: "; password = readPasswordMasked();
        cout << "Confirm password: "; confirm = readPasswordMasked();
        if (password != confirm) {
            cout << "Passwords do not match. Try again.\n";
            continue;
        }
        if (password.empty()) { cout << "Password cannot be empty.\n"; continue; }
        break;
    }

    // collect customer info
    string name, phone, address, email;
    cout << "Enter your full name: "; getline(cin, name);
    cout << "Enter phone number: "; getline(cin, phone);
    cout << "Enter address: "; getline(cin, address);
    cout << "Enter email: "; getline(cin, email);

    // generate unique customer ID of form C###
    int idx = customerDAO.getDataCache().getSize() + 1;
    string custID;
    auto pad = [](int x) {
        char buf[16];
        sprintf(buf, "C%03d", x);
        return string(buf);
    };
    do {
        custID = pad(idx);
        idx++;
    } while (customerDAO.read(custID) != nullptr);

    Customer* c = new Customer(custID, name, phone, address, email);
    if (!customerDAO.create(custID, c)) {
        cout << "Failed to create customer record. Registration aborted.\n";
        delete c;
        return;
    }

    Account* acc = new Account();
    acc->setUsername(username);
    acc->setPassword(password);
    acc->setUserType("customer");
    acc->setUserID(custID);
    if (!accountDAO.create(username, acc)) {
        cout << "Failed to create account. Rolling back customer record.\n";
        customerDAO.remove(custID);
        delete acc;
        return;
    }

    cout << "Registration successful! Your customer ID: " << custID << "\n";
    cout << "You can login with username '" << username << "'.\n";
}