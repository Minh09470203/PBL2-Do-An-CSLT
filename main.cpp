#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "Models/Account.h"
#include "Models/Person.h"
#include "Models/Customer.h"
#include "Models/Staff.h"
#include "Models/Supplier.h"
#include "Models/Category.h"
#include "Models/Invoice.h"
#include "Models/Product.h"
#include "AppSession/SessionManager.h"
/*
             _ooOoo_
            o8888888o
            88" . "88
            (| -_- |)
            O\  =  /O
         ____/`---'\____
       .'  \\|     |//  `.
      /  \\|||  :  |||//  \
     /  _||||| -:- |||||-  \
     |   | \\\  -  /// |   |
     | \_|  ''\---/''  |   |
     \  .-\__  `-`  ___/-. /
   ___`. .'  /--.--\  `. . __
."" '<  `.___\_<|>_/___.'  >'"".
| | :  `- \`.;`\ _ /`;.`/ - ` : | |
\  \ `-.   \_ __\ /__ _/   .-` /  /
 `-.____`-.___\_____/___.-`____.-'
              `=---='
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Phật phù hộ, không bao giờ BUG
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/


// Menu Staff
void StaffMenu(CustomerDAO &customerDao, StaffDAO &staffDao,
               ProductDAO &productDao, InvoiceDAO &invoiceDao,
               AccountDAO &accountDao, CategoryDAO &categoryDao,
               SupplierDAO &supplierDao) {
    SessionManager& session = SessionManager::getInstance();
    string position = session.getCurrentStaff()->getPosition();
    bool isManager = (position == "Manager");
    
    int choice;
    do {
        cout << "\n========== STAFF MENU ==========\n";
        cout << "Staff: " << session.getCurrentStaff()->getNamenv() << " (" << position << ")\n";
        
        if (isManager) {
            // Manager - Full menu
            cout << "1. Customer Management\n";
            cout << "2. Product Management\n";
            cout << "3. Invoice Management\n";
            cout << "4. Category Management\n";
            cout << "5. Supplier Management\n";
            cout << "6. View Statistics\n";
            cout << "7. Change Password\n";
        } else {
            // Staff thường - Limited menu
            cout << "1. View Customer List\n";
            cout << "2. View Product List\n";
            cout << "3. Create Invoice\n";
            cout << "4. Change Password\n";
        }
        
        cout << "0. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (isManager) {
            // Manager menu handling
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
                        printCustomerList(customerDao);
                    } else if (cusChoice == 2) {
                        CustomerInsert(customerDao);
                    } else if (cusChoice == 3) {
                        CustomerSearch(customerDao);
                    }
                    break;
                }
                case 2: {
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
                        printProductList(productDao);
                    } else if (prodChoice == 2) {
                        ProductInsert(productDao, supplierDao, categoryDao);
                    } else if (prodChoice == 3) {
                        ProductDelete(productDao);
                    } else if (prodChoice == 4) {
                        string id;
                        cout << "Enter product ID to search: ";
                        getline(cin, id);
                        Product* p = ProductSearchByID(productDao, id);
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
                case 3: {
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
                        printInvoiceList(invoiceDao);
                    } else if (invChoice == 2) {
                        CreateInvoice(invoiceDao, customerDao, staffDao, productDao);
                    } else if (invChoice == 3) {
                        string invID;
                        cout << "Enter invoice ID: ";
                        getline(cin, invID);
                        Invoice* inv = invoiceDao.read(invID);  // use instance
                        if (inv) {
                            inv->PrintHD();
                        } else {
                            cout << "Invoice not found!\n";
                        }
                    }   
                    break;
                }
                case 4: {
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
                        ShowCategoryList(categoryDao);
                    } else if(cateChoice == 2){
                        AddCategory(categoryDao);
                    }
                    break;
                }
                case 5: {
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
                        ShowSupplierList(supplierDao);
                    } else if(supChoice == 2){
                        AddSupplier(supplierDao);
                    }
                    break;
                }
                case 6: {
                    cout << "\n--- Statistics ---\n";
                    cout << "Total customers: " << customerDao.getDataCache().getSize() << endl;
                    cout << "Total staff: " << staffDao.getDataCache().getSize() << endl;
                    cout << "Total products: " << productDao.getDataCache().getSize() << endl;
                    cout << "Total invoices: " << invoiceDao.getDataCache().getSize() << endl;

                    unsigned long totalRevenue = 0;
                    for (int i = 0; i < invoiceDao.getDataCache().getSize(); i++) {
                        totalRevenue += invoiceDao.getDataCache()[i]->getTotalAmount();
                    }
                    cout << "Total revenue: " << totalRevenue << " VND" << endl;
                    break;
                }
                case 7:
                    ChangePassword(*session.getAccountDAO());
                    break;
                case 0:
                    session.logout();
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } else {
            // Staff thường menu handling
            switch(choice) {
                case 1: 
                    printCustomerList(customerDao); 
                    break;
                    
                case 2: 
                    printProductList(productDao); 
                    break;
                    
                case 3: 
                    CreateInvoice(invoiceDao, customerDao, staffDao, productDao); 
                    break;
                    
                case 4: 
                    ChangePassword(accountDao); 
                    break;
                    
                case 0: 
                    session.logout(); 
                    break;
                    
                default: 
                    cout << "Invalid choice!\n";
            }
        }
    } while (choice != 0);
}

// Menu Customer
void CustomerMenu(ProductDAO &productDao,
                  InvoiceDAO &invoiceDao,
                  CustomerDAO &customerDao,
                  AccountDAO &accountDao) {
    SessionManager& session = SessionManager::getInstance();
    int choice;
    do {
        cout << "\n========== CUSTOMER MENU ==========\n";
        cout << "Welcome: " << session.getCurrentCustomer()->getName() << "\n";
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
                printProductList(productDao);
                break;
            case 2: {
                string keyword;
                cout << "Enter product name or ID: ";
                getline(cin, keyword);
                bool found = false;
                for (int i = 0; i < productDao.getDataCache().getSize(); i++) {
                    if (productDao.getDataCache()[i]->getIDsp() == keyword || productDao.getDataCache()[i]->getNamesp() == keyword) {
                        cout << "Product found:\n";
                        cout << left << setw(8)  << "ID"
                             << setw(20) << "Shoe Name"
                             << setw(6)  << "Size"
                             << setw(12) << "Price"
                             << setw(8) << "Color"
                             << setw(18) << "Stock Quantity"
                             << setw(15) << "Category"
                             << setw(15) << "Supplier" << endl;
                        productDao.getDataCache()[i]->XuatSP();
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
                for (int i = 0; i < invoiceDao.getDataCache().getSize(); i++) {
                    if (invoiceDao.getDataCache()[i]->getCustomer()->getID() == session.getCurrentCustomer()->getID()) {
                        cout << "Invoice: " << invoiceDao.getDataCache()[i]->getIDhd()
                             << " | Date: " << invoiceDao.getDataCache()[i]->getDate()
                             << " | Total: " << invoiceDao.getDataCache()[i]->getTotalAmount() << " VND" << endl;
                        found = true;
                    }
                }
                if (!found) {
                    cout << "No invoices found.\n";
                }
                break;
            }
            case 4: {
                CustomerEdit(session.getCurrentCustomer(), customerDao);
                break;
            }
            case 5:
                ChangePassword(accountDao);
                break;
            case 0:
                session.logout();
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 0);
}

int main() {
    SessionManager& session = SessionManager::getInstance();
    session.loadAllData();
    cout << "===> SHOE STORE MANAGEMENT APPLICATION <===" << endl;
    cout << "Customers: " << session.getCustomerDAO()->getDataCache().getSize() << endl;
    cout << "Staff: " << session.getStaffDAO()->getDataCache().getSize() << endl;
    cout << "Products: " << session.getProductDAO()->getDataCache().getSize() << endl;
    cout << "Invoices: " << session.getInvoiceDAO()->getDataCache().getSize() << endl;

    int userType;
    do {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Login\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> userType;
        cin.ignore();

        if (userType == 1) {
            int loginResult = Login(*session.getCustomerDAO(), *session.getStaffDAO(), *session.getAccountDAO());
            if (loginResult == 1) {
                StaffMenu(*session.getCustomerDAO(), *session.getStaffDAO(), *session.getProductDAO(), *session.getInvoiceDAO(), *session.getAccountDAO(), *session.getCategoryDAO(), *session.getSupplierDAO());
            } else if (loginResult == 2) {
                CustomerMenu(*session.getProductDAO(), *session.getInvoiceDAO(), *session.getCustomerDAO(), *session.getAccountDAO());
            }
        } else if (userType == 0) {
            cout << "Thank you for using our system!\n";
        } else {
            cout << "Invalid choice!\n";
        }
    } while (userType != 0);
}
