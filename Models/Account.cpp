#include "Account.h"
#include "../AppSession/SessionManager.h"

// Change password function
void ChangePassword(AccountDAO &accountDAO) {
    SessionManager& session = SessionManager::getInstance();
    string currentUsername = session.getCurrentUsername();
    
    string oldPassword, newPassword, confirmPassword;
    cout << "Enter old password: "; getline(cin, oldPassword);
    cout << "Enter new password: "; getline(cin, newPassword);
    cout << "Confirm new password: "; getline(cin, confirmPassword);

    if (newPassword != confirmPassword) {
        cout << "Passwords do not match!\n";
        return;
    }

    Account* account = accountDAO.read(currentUsername);
    if (account && account->password == oldPassword) {
        account->password = newPassword;  
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
    cout << "Password: "; getline(cin, password);

    SessionManager& session = SessionManager::getInstance();

    Account* account = accountDAO.read(username);
    
    if (account && account->password == password) {
        session.setCurrentUsername(username);
        
        if (account->userType == "staff") {
            // Find staff
            Staff* staff = staffDAO.read(account->userID);
            if (staff) {
                session.setCurrentStaff(staff);
                cout << "Staff login successful! Welcome " << session.getCurrentStaff()->getNamenv() << "\n";
                return 1; // Staff
            } else {
                cout << "Staff data not found!\n";
                return 0;
            }
        }
        else if (account->userType == "customer") {
            // Find customer
            Customer* customer = customerDAO.read(account->userID);
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
