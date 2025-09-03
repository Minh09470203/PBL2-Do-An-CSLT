#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Customer.h"
#include "Staff.h"
#include "Supplier.h"
#include "Category.h"
#include "Invoice.h"
#include "Product.h"


Customer* currentCustomer = nullptr;
Staff* currentStaff = nullptr;
string currentUsername = "";


struct Account {
    string username;
    string password;
    string userType; // "admin", "staff", "customer"
    string userID;   // ID user
};

void AddAccount(MyVector<Account*> &accounts) {
    string username, password, userType, userID;
    cout << "Enter username: "; getline(cin, username);
    cout << "Enter password: "; getline(cin, password);
    cout << "Enter user type (admin/staff/customer): "; getline(cin, userType);
    cout << "Enter user ID: "; getline(cin, userID);

    // Check for duplicate usernames
    for (int i = 0; i < accounts.getSize(); i++) {
        if (accounts[i]->username == username) {
            cout << "Username already exists!\n";
            return;
        }
    }

    Account *acc = new Account();
    acc->username = username;
    acc->password = password;
    acc->userType = userType;
    acc->userID = userID;
    accounts.Push_back(acc);

    ofstream file("Data/Accounts.txt");
    if (file.is_open()) {
        for (int i = 0; i < accounts.getSize(); i++) {
            file << accounts[i]->username << "|"
                 << accounts[i]->password << "|"
                 << accounts[i]->userType << "|"
                 << accounts[i]->userID << "\n";
        }
        file.close();
        cout << "Account added successfully!\n";
    } else {
        cout << "Cannot save to Accounts.txt\n";
    }
}

// Change password function
void ChangePassword(MyVector<Account*> &accounts, string currentUsername) {
    string oldPassword, newPassword, confirmPassword;
    cout << "Enter old password: "; getline(cin, oldPassword);
    cout << "Enter new password: "; getline(cin, newPassword);
    cout << "Confirm new password: "; getline(cin, confirmPassword);

    if (newPassword != confirmPassword) {
        cout << "Passwords do not match!\n";
        return;
    }

    for (int i = 0; i < accounts.getSize(); i++) {
        if (accounts[i]->username == currentUsername && accounts[i]->password == oldPassword) {
            accounts[i]->password = newPassword;

            // Save to file
            ofstream file("Accounts.txt");
            if (file.is_open()) {
                for (int j = 0; j < accounts.getSize(); j++) {
                    file << accounts[j]->username << "|"
                         << accounts[j]->password << "|"
                         << accounts[j]->userType << "|"
                         << accounts[j]->userID << "\n";
                }
                file.close();
                cout << "Password changed successfully!\n";
            } else {
                cout << "Cannot save to Accounts.txt\n";
            }
            return;
        }
    }
    cout << "Old password is incorrect!\n";
}
// Load accounts from file
bool loadAccounts(MyVector<Account*> &accounts) {
    ifstream file("Data/Accounts.txt");
    if (!file.is_open()) {
        cout << "Cannot open Accounts.txt" << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string username, password, userType, userID;
        getline(ss, username, '|');
        getline(ss, password, '|');
        getline(ss, userType, '|');
        getline(ss, userID);

        Account *acc = new Account();
        acc->username = username;
        acc->password = password;
        acc->userType = userType;
        acc->userID = userID;
        accounts.Push_back(acc);
    }
    file.close();
    return true;
}

// Login function
int Login(MyVector<Customer*> &cus, MyVector<Staff*> &sta, MyVector<Account*> &accounts) {
    string username, password;
    cout << "\n========== LOGIN ==========\n";
    cout << "Username: "; getline(cin, username);
    cout << "Password: "; getline(cin, password);

    // Account search
    for (int i = 0; i < accounts.getSize(); i++) {
        if (accounts[i]->username == username && accounts[i]->password == password) {
            currentUsername = username;
            if (accounts[i]->userType == "admin") {
                cout << "Admin login successful!\n";
                return 1; // Admin
            }
            else if (accounts[i]->userType == "staff") {
                // Find staff
                for (int j = 0; j < sta.getSize(); j++) {
                    if (sta[j]->getIDnv() == accounts[i]->userID) {
                        currentStaff = sta[j];
                        cout << "Staff login successful! Welcome " << currentStaff->getNamenv() << "\n";
                        return 2; // Staff
                    }
                }
                cout << "Staff data not found!\n";
                return 0;
            }
            else if (accounts[i]->userType == "customer") {
                // Find customer
                for (int j = 0; j < cus.getSize(); j++) {
                    if (cus[j]->getID() == accounts[i]->userID) {
                        currentCustomer = cus[j];
                        cout << "Customer login successful! Welcome " << currentCustomer->getName() << "\n";
                        return 3; // Customer
                    }
                }
                cout << "Customer data not found!\n";
                return 0;
            }
        }
    }

    cout << "Invalid username or password!\n";
    return 0; // Login failed
}

// Menu Admin
void AdminMenu(MyVector<Customer*> &cus, MyVector<Staff*> &sta,
               MyVector<Product*> &sp, MyVector<Invoice*> &invoices,
               MyVector<Account*> &accounts) {
    int choice;
    do {
        cout << "\n========== ADMIN MENU ==========\n";
        cout << "1. Customer Management\n";
        cout << "2. Staff Management\n";
        cout << "3. Product Management\n";
        cout << "4. Invoice Management\n";
        cout << "5. Category Management\n";
        cout << "6. Supplier Management\n";
        cout << "7. Account Management\n";
        cout << "8. View Statistics\n";
        cout << "9. Change Password\n";
        cout << "0. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: {
                int cusChoice;
                cout << "\n--- Customer Management ---\n";
                cout << "1. View all customers\n";
                cout << "2. Add customer\n";
                cout << "3. Search customer\n";
                do {
                    cout << "Enter choice: ";
                    cin >> cusChoice;
                    if (cusChoice < 1 || cusChoice > 3) cout << "Invalid choice!" << endl;
                }while(cusChoice < 1 || cusChoice > 3);
                cin.ignore();

                if (cusChoice == 1) {
                    printCustomerList(cus);
                } else if (cusChoice == 2) {
                    int n;
                    cout << "Enter number of customers to add: ";
                    cin >> n;
                    cin.ignore();
                    CustomerInsert(cus, n);
                } else if (cusChoice == 3) {
                    CustomerSearch(cus);
                }
                break;
            }
            case 2: {
                int staffChoice;
                cout << "\n--- Staff Management ---\n";
                cout << "1. View all staff\n";
                cout << "2. Add staff\n";
                cout << "3. Delete staff\n";
                do {
                    cout << "Enter choice: ";
                    cin >> staffChoice;
                    if (staffChoice < 1 || staffChoice > 3) cout << "Invalid choice!" << endl;
                }while(staffChoice < 1 || staffChoice > 3);
                cin.ignore();

                if (staffChoice == 1) {
                    printStaffList(sta);
                } else if (staffChoice == 2) {
                    int n;
                    cout << "Enter number of staff to add: ";
                    cin >> n;
                    cin.ignore();
                    StaffInsert(sta, n);
                } else if (staffChoice == 3) {
                    StaffDelete(sta);
                }
                break;
            }
            case 3: {
                int prodChoice;
                cout << "\n--- Product Management ---\n";
                cout << "1. View all products\n";
                cout << "2. Add product\n";
                cout << "3. Delete product\n";
                cout << "4. Search product\n";
                do {
                    cout << "Enter choice: ";
                    cin >> prodChoice;
                    if (prodChoice < 1 || prodChoice > 4) cout << "Invalid choice!" << endl;
                }while(prodChoice < 1 || prodChoice > 4);
                cin.ignore();

                if (prodChoice == 1) {
                    printProductList(sp);
                } else if (prodChoice == 2) {
                    int n;
                    cout << "Enter number of products to add: ";
                    cin >> n;
                    cin.ignore();
                    ProductInsert(sp, n);
                } else if (prodChoice == 3) {
                    ProductDelete(sp);
                } else if (prodChoice == 4) {
                    string id;
                    cout << "Enter product ID to search: ";
                    getline(cin, id);
                    Product* p = ProductSearchByID(sp, id);
                    if (p) {
                        cout << "Product found:\n";
                        cout << left << setw(8)  << "ID"
                             << setw(20) << "Shoe Name"
                             << setw(6)  << "Size"
                             << setw(12) << "Price"
                             << setw(8) << "Color"
                             << setw(18) << "Stock Quantity"
                             << setw(15) << "Category"
                             << setw(15) << "Supplier" << endl;
                        p->XuatSP();
                    } else {
                        cout << "Product not found!\n";
                    }
                }
                break;
            }
            case 4: {
                int invChoice;
                cout << "\n--- Invoice Management ---\n";
                cout << "1. View all invoices\n";
                cout << "2. Create invoice\n";
                cout << "3. View invoice detail\n";
                do {
                    cout << "Enter choice: ";
                    cin >> invChoice;
                    if (invChoice < 1 || invChoice > 3) cout << "Invalid choice!" << endl;
                }while(invChoice < 1 || invChoice > 3);
                cin.ignore();

                if (invChoice == 1) {
                    printInvoiceList(invoices);
                } else if (invChoice == 2) {
                    CreateInvoice(invoices, cus, sta, sp);
                } else if (invChoice == 3) {
                    string invID;
                    cout << "Enter invoice ID: ";
                    getline(cin, invID);
                    for (int i = 0; i < invoices.getSize(); i++) {
                        if (invoices[i]->getIDhd() == invID) {
                            invoices[i]->PrintHD();
                            break;
                        }
                    }
                }
                break;
            }
            case 5: {
                int cateChoice;
                cout << "\n--- Category Management ---\n";
                cout << "1. View all categories\n";
                cout << "2. Add category\n";
                do {
                    cout << "Enter choice: ";
                    cin >> cateChoice;
                    if (cateChoice < 1 || cateChoice > 2) cout << "Invalid choice!" << endl;
                }while(cateChoice < 1 || cateChoice > 2);
                cin.ignore();
                if (cateChoice == 1) {
                    ShowCategoryList(categoryMap);
                } else if(cateChoice == 2){
                    AddCategory(categoryMap);
                }
                break;
            }
            case 6: {
                int supChoice;
                cout << "\n--- Supplier Management ---\n";
                cout << "1. View all suppliers\n";
                cout << "2. Add supplier\n";
                do {
                    cout << "Enter choice: ";
                    cin >> supChoice;
                    if (supChoice< 1 || supChoice > 2) cout << "Invalid choice!" << endl;
                }while(supChoice < 1 || supChoice > 2);
                cin.ignore();
                if (supChoice == 1) {
                    ShowSupplierList(supplierMap);
                } else if(supChoice == 2){
                    AddSupplier(supplierMap);
                }
                break;
            }
            case 7: {
                int accChoice;
                cout << "\n--- Account Management ---\n";
                cout << "1. View all accounts\n";
                cout << "2. Add account\n";
                cout << "3. Delete account\n";
                do {
                    cout << "Enter choice: ";
                    cin >> accChoice;
                    if (accChoice < 1 || accChoice > 3) cout << "Invalid choice!" << endl;
                }while(accChoice < 1 || accChoice > 3);
                cin.ignore();

                if (accChoice == 1) {
                    cout << left << setw(15) << "Username"
                         << setw(15) << "Password"
                         << setw(10) << "Type"
                         << setw(10) << "User ID" << endl;
                    cout << string(50, '-') << endl;
                    for (int i = 0; i < accounts.getSize(); i++) {
                        cout << left << setw(15) << accounts[i]->username
                             << setw(15) << "******"
                             << setw(10) << accounts[i]->userType
                             << setw(10) << accounts[i]->userID << endl;
                    }
                } else if (accChoice == 2) {
                    AddAccount(accounts);
                } else if (accChoice == 3) {
                    string username;
                    cout << "Enter username to delete: ";
                    getline(cin, username);
                    if (username == "admin") {
                        cout << "Cannot delete admin account!\n";
                        break;
                    }
                    bool found = false;
                    for (int i = 0; i < accounts.getSize(); i++) {
                        if (accounts[i]->username == username) {
                            delete accounts[i];
                            for (int j = i; j < accounts.getSize() - 1; j++) {
                                accounts[j] = accounts[j + 1];
                            }
                            found = true;
                            // Save to file
                            ofstream file("Accounts.txt");
                            if (file.is_open()) {
                                for (int k = 0; k < accounts.getSize() - 1; k++) {
                                    file << accounts[k]->username << "|"
                                         << accounts[k]->password << "|"
                                         << accounts[k]->userType << "|"
                                         << accounts[k]->userID << "\n";
                                }
                                file.close();
                                cout << "Account deleted successfully!\n";
                            }
                            break;
                        }
                    }
                    if (!found) {
                        cout << "Account not found!\n";
                    }
                }
                break;
            }
            case 8: {
                cout << "\n--- Statistics ---\n";
                cout << "Total customers: " << cus.getSize() << endl;
                cout << "Total staff: " << sta.getSize() << endl;
                cout << "Total products: " << sp.getSize() << endl;
                cout << "Total invoices: " << invoices.getSize() << endl;

                unsigned long totalRevenue = 0;
                for (int i = 0; i < invoices.getSize(); i++) {
                    totalRevenue += invoices[i]->getTotalAmount();
                }
                cout << "Total revenue: " << totalRevenue << " VND" << endl;
                break;
            }
            case 9:
                ChangePassword(accounts, currentUsername);
                break;
            case 0:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 0);
}

// Menu Staff
void StaffMenu(MyVector<Customer*> &cus, MyVector<Staff*> &sta,
               MyVector<Product*> &sp, MyVector<Invoice*> &invoices,
               MyVector<Account*> &accounts) {
    int choice;
    do {
        cout << "\n========== STAFF MENU ==========\n";
        cout << "Staff: " << currentStaff->getNamenv() << " (" << currentStaff->getPosition() << ")\n";
        cout << "1. View products\n";
        cout << "2. Search product\n";
        cout << "3. Create invoice\n";
        cout << "4. View my invoices\n";
        cout << "5. View customers\n";
        cout << "6. View all categories\n";
        cout << "7. View all suppliers\n";
        cout << "8. Change Password\n";
        cout << "0. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1:
                printProductList(sp);
                break;
            case 2: {
                string id;
                cout << "Enter product ID to search: ";
                getline(cin, id);
                Product* p = ProductSearchByID(sp, id);
                if (p) {
                    cout << "Product found:\n";
                    cout << left << setw(8)  << "ID"
                         << setw(20) << "Shoe Name"
                         << setw(6)  << "Size"
                         << setw(12) << "Price"
                         << setw(8) << "Color"
                         << setw(18) << "Stock Quantity"
                         << setw(15) << "Category"
                         << setw(15) << "Supplier" << endl;
                    p->XuatSP();
                } else {
                    cout << "Product not found!\n";
                }
                break;
            }
            case 3:
                CreateInvoice(invoices, cus, sta, sp);
                break;
            case 4: {
                cout << "\n--- Your invoices ---\n";
                bool found = false;
                for (int i = 0; i < invoices.getSize(); i++) {
                    if (invoices[i]->getStaff()->getIDnv() == currentStaff->getIDnv()) {
                        cout << "Invoice: " << invoices[i]->getIDhd()
                             << " | Date: " << invoices[i]->getDate()
                             << " | Total: " << invoices[i]->getTotalAmount() << " VND" << endl;
                        found = true;
                    }
                }
                if (!found) {
                    cout << "No invoices found.\n";
                }
                break;
            }
            case 5:
                printCustomerList(cus);
                break;
            case 6:
                ShowCategoryList(categoryMap);
                break;
            case 7:
                ShowSupplierList(supplierMap);
                break;
            case 8:
                ChangePassword(accounts, currentUsername);
                break;
            case 0:
                cout << "Logging out...\n";
                currentStaff = nullptr;
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 0);
}

// Menu Customer
void CustomerMenu(MyVector<Product*> &sp, MyVector<Invoice*> &invoices,
                  MyVector<Customer*> &cus, MyVector<Account*> &accounts) {
    int choice;
    do {
        cout << "\n========== CUSTOMER MENU ==========\n";
        cout << "Welcome: " << currentCustomer->getName() << "\n";
        cout << "1. View products\n";
        cout << "2. Search product\n";
        cout << "3. View my invoices\n";
        cout << "4. Update personal information\n";
        cout << "5. Change Password\n";
        cout << "0. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1:
                printProductList(sp);
                break;
            case 2: {
                string keyword;
                cout << "Enter product name or ID: ";
                getline(cin, keyword);
                bool found = false;
                for (int i = 0; i < sp.getSize(); i++) {
                    if (sp[i]->getIDsp() == keyword || sp[i]->getNamesp() == keyword) {
                        cout << "Product found:\n";
                        cout << left << setw(8)  << "ID"
                             << setw(20) << "Shoe Name"
                             << setw(6)  << "Size"
                             << setw(12) << "Price"
                             << setw(8) << "Color"
                             << setw(18) << "Stock Quantity"
                             << setw(15) << "Category"
                             << setw(15) << "Supplier" << endl;
                        sp[i]->XuatSP();
                        found = true;
                    }
                }
                if (!found) {
                    cout << "Product not found!\n";
                }
                break;
            }
            case 3: {
                cout << "\n--- Your invoices ---\n";
                bool found = false;
                for (int i = 0; i < invoices.getSize(); i++) {
                    if (invoices[i]->getCustomer()->getID() == currentCustomer->getID()) {
                        cout << "Invoice: " << invoices[i]->getIDhd()
                             << " | Date: " << invoices[i]->getDate()
                             << " | Total: " << invoices[i]->getTotalAmount() << " VND" << endl;
                        found = true;
                    }
                }
                if (!found) {
                    cout << "No invoices found.\n";
                }
                break;
            }
            case 4: {
                CustomerEdit(currentCustomer, cus);
                break;
            }
            case 5:
                ChangePassword(accounts, currentUsername);
                break;
            case 0:
                cout << "Logging out...\n";
                currentCustomer = nullptr;
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 0);
}

int main() {
    MyVector<Customer*> cus;
    MyVector<Staff*> sta;
    MyVector<Product*> sp;
    MyVector<Invoice*> invoices;
    MyVector<Account*> accounts;

    cout << "Loading data from files...\n";
    int loadedFiles = 0;
    int totalFiles = 7;

    if (loadAccounts(accounts)) loadedFiles++;
    if (loadCustomerFile("Data/Customer.txt", cus)) loadedFiles++;
    if (loadStaffFile("Data/Staff.txt", sta)) loadedFiles++;
    if (loadCategoryMap("Data/Category.txt")) loadedFiles++;
    if (loadSupplierMap("Data/Supplier.txt")) loadedFiles++;
    if (loadProductFile("Data/Product.txt", sp)) loadedFiles++;
    if (loadInvoiceFile("Data/Invoice.txt", invoices, cus, sta, sp)) loadedFiles++;
    if (loadedFiles == totalFiles) {
        cout << "All data loaded successfully!\n" << endl;
    } else {
    cout << "Warning!!! " << endl;
    cout << "Some data may be missing.\n" << endl;
    }

    cout << "===> SHOE STORE MANAGEMENT APPLICATION <===" << endl;
    cout << "Customers: " << cus.getSize() << endl;
    cout << "Staff: " << sta.getSize() << endl;
    cout << "Products: " << sp.getSize() << endl;
    cout << "Invoices: " << invoices.getSize() << endl;

    int userType;
    do {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Login\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> userType;
        cin.ignore();

        if (userType == 1) {
            int loginResult = Login(cus, sta, accounts);
            // Handle login results with proper menu calls
            if (loginResult == 1) {
                AdminMenu(cus, sta, sp, invoices, accounts);
            } else if (loginResult == 2) {
                StaffMenu(cus, sta, sp, invoices, accounts);
            } else if (loginResult == 3) {
                CustomerMenu(sp, invoices, cus, accounts);
            } else if (userType == 0) {
            cout << "Thank you for using our system!\n";
            }
            else {
            cout << "Invalid choice!\n";
            }
        }
    } while (userType != 0);

    // Memory cleanup
    for (int i = 0; i < cus.getSize(); i++) delete cus[i];
    for (int i = 0; i < sta.getSize(); i++) delete sta[i];
    for (int i = 0; i < sp.getSize(); i++) delete sp[i];
    for (int i = 0; i < invoices.getSize(); i++) delete invoices[i];
    for (int i = 0; i < accounts.getSize(); i++) delete accounts[i];

    return 0;
}
