#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "DataStructure/MyVector.h"
#include <iomanip>
#include "Models/Account.h"
#include "Models/Person.h"
#include "Models/Customer.h"
#include "Models/Staff.h"
#include "Models/Supplier.h"
#include "Models/Category.h"
#include "Models/Order.h"
#include "Models/Product.h"
#include "AppSession/SessionManager.h"
#include "Models/StatsService.h"

// Enable UTF-8 
#ifdef _WIN32
#include <windows.h>
static void enableUtf8Console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
}
#else
static void enableUtf8Console() { /* no-op on non-Windows */ }
#endif
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
               ProductDAO &productDao, OrderDAO &orderDao,
               AccountDAO &accountDao, CategoryDAO &categoryDao,
               SupplierDAO &supplierDao) {
    SessionManager& session = SessionManager::getInstance();
    string position = session.getCurrentStaff()->getPosition();
    bool isManager = (position == "Manager");

    int choice;
    do {
        cout << "\n========== STAFF MENU ==========" << "\n";
        cout << "Staff: " << session.getCurrentStaff()->getNamenv() << " (" << position << ")" << "\n";

        if (isManager) {
            // Manager - Full menu
            cout << "1. Customer Management\n";
            cout << "2. Product Management\n";
            cout << "3. Order Management\n";
            cout << "4. Category Management\n";
            cout << "5. Supplier Management\n";
            cout << "6. View Statistics\n";
            cout << "7. Change Password\n";
        } else {
            // Normal staff - Limited menu (no create-order)
            cout << "1. View Customer List\n";
            cout << "2. View Product List\n";
            cout << "3. Order Requests\n";
            cout << "4. Order Status\n";
            cout << "5. Change Password\n";
        }

        cout << "0. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (isManager) {
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
                    } while(cusChoice < 1 || cusChoice > 3);
                    cin.ignore();

                    if (cusChoice == 1) printCustomerList(customerDao);
                    else if (cusChoice == 2) CustomerInsert(customerDao);
                    else if (cusChoice == 3) CustomerSearch(customerDao);
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
                    } while(prodChoice < 1 || prodChoice > 4);
                    cin.ignore();

                    if (prodChoice == 1) printProductList(productDao);
                    else if (prodChoice == 2) ProductInsert(productDao, supplierDao, categoryDao);
                    else if (prodChoice == 3) ProductDelete(productDao);
                    else if (prodChoice == 4) {
                        int sChoice;
                        cout << "1. Search by ID or Name\n";
                        cout << "2. Search by range of Price\n";
                        cin >> sChoice; cin.ignore();
                        if (sChoice == 1) {
                            string keyword;
                            cout << "Enter product ID or name: ";
                            getline(cin, keyword);
                            Product* p = ProductSearchByIDName(productDao, keyword);
                            if (p) { printHeader(); cout << *p << endl; }
                            else cout << "Product not found!\n";
                        } else if (sChoice == 2) {
                            SearchProductByPriceRange(productDao);
                        } else {
                            cout << "Invalid choice!\n";
                        }
                    }
                    break;
                }
                case 3: {
                    int invChoice;
                    cout << "\n--- Order Management ---\n";
                    cout << "1. View all orders\n";
                    cout << "2. View order detail\n";
                    cout << "3. Order requests\n";
                    cout << "4. Order status\n";
                    do {
                        cout << "Enter choice: ";
                        cin >> invChoice;
                        if (invChoice < 1 || invChoice > 5) cout << "Invalid choice!" << endl;
                    } while(invChoice < 1 || invChoice > 5);
                    cin.ignore();

                    if (invChoice == 1) {
                        printOrderList(orderDao);
                    } else if (invChoice == 2) {
                        string ordID; cout << "Enter order ID: "; getline(cin, ordID);
                        Order* ord = orderDao.read(ordID);
                        if (ord) ord->PrintHD(); else cout << "Order not found!\n";
                    } else if (invChoice == 3) {
                        ProcessOrderRequests(orderDao, session.getCurrentStaff());
                    } else if (invChoice == 4) {
                        HandleOrderStatusUpdate(orderDao);
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
                    } while(cateChoice < 1 || cateChoice > 2);
                    cin.ignore();
                    if (cateChoice == 1) ShowCategoryList(categoryDao);
                    else if (cateChoice == 2) AddCategory(categoryDao);
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
                        if (supChoice < 1 || supChoice > 2) cout << "Invalid choice!" << endl;
                    } while(supChoice < 1 || supChoice > 2);
                    cin.ignore();
                    if (supChoice == 1) ShowSupplierList(supplierDao);
                    else if (supChoice == 2) AddSupplier(supplierDao);
                    break;
                }
                case 6: {
                    StatsService stats(session);
                    int sChoice = -1;
                    do {
                        cout << "\n--- Statistics ---\n";
                        cout << "1. Dashboard (summary)\n";
                        cout << "2. Revenue by month (year)\n";
                        cout << "3. Top products by quantity\n";
                        cout << "4. Top customers by revenue\n";
                        cout << "5. Low-stock products\n";
                        cout << "0. Back\n";
                        cout << "Enter your choice: ";
                        cin >> sChoice; cin.ignore();
                        if (sChoice == 1) stats.printDashboard();
                        else if (sChoice == 2) { int year; cout << "Enter year: "; cin >> year; cin.ignore(); stats.printRevenueByMonth(year); }
                        else if (sChoice == 3) { int N; cout << "Top how many products? "; cin >> N; cin.ignore(); stats.printTopProductsByQty(N); }
                        else if (sChoice == 4) { int N; cout << "Top how many customers? "; cin >> N; cin.ignore(); stats.printTopCustomersByRevenue(N); }
                        else if (sChoice == 5) { int threshold; cout << "Show products with stock <= "; cin >> threshold; cin.ignore(); stats.printLowStock(threshold); }
                        else if (sChoice != 0) cout << "Invalid choice!\n";
                    } while (sChoice != 0);
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
            // Normal staff menu handling
            switch(choice) {
                case 1: printCustomerList(customerDao); break;
                case 2: printProductList(productDao); break;
                case 3: ProcessOrderRequests(orderDao, session.getCurrentStaff()); break;
                case 4: HandleOrderStatusUpdate(orderDao); break;
                case 5: ChangePassword(accountDao); break;
                case 0: session.logout(); break;
                default: cout << "Invalid choice!\n";
            }
        }
    } while (choice != 0);
}

// Menu Customer
void CustomerMenu(ProductDAO &productDao,
                  OrderDAO &orderDao,
                  CustomerDAO &customerDao,
                  AccountDAO &accountDao) {
    SessionManager& session = SessionManager::getInstance(); 
    int choice;
    do {
        cout << "\n========== CUSTOMER MENU ==========\n";
        cout << "Welcome: " << session.getCurrentCustomer()->getName() << "\n";
    cout << "1. View products\n";
    cout << "2. Search product\n";
    cout << "3. View my orders\n";
    cout << "4. Place Order\n";
    cout << "5. Update personal information\n";
    cout << "6. Change Password\n";
        cout << "0. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1:
                printProductList(productDao);
                break;
            case 2: {
                int choice;
                cout << "1. Search by ID or Name\n";
                cout << "2. Search by range of Price\n";
                cout << "Enter your choice: ";
                cin >> choice;
                cin.ignore();
                if (choice == 1) {
                    string keyword;
                    cout << "Enter product ID or name: ";
                    getline(cin, keyword);
                    Product* p = ProductSearchByIDName(productDao, keyword);
                    if (p) {
                    cout << "Product found:\n";
                    printHeader();
                    cout << *p << endl;
                } else {
                    cout << "Product not found!\n";
                }
                } else if (choice == 2) {
                    SearchProductByPriceRange(productDao);
                } else {
                    cout << "Invalid choice!\n";
                }
                break;
            }
            case 3: {
                cout << "\n--- Your orders ---\n";
                bool found = false;
                MyVector<Order*>& allInv = orderDao.getDataCache();
                MyVector<Order*> pending; // pending for this customer
                for (int i = 0; i < allInv.getSize(); i++) {
                    Order* ord = allInv[i];
                    if (ord->getCustomer() && ord->getCustomer()->getID() == session.getCurrentCustomer()->getID()) {
                        cout << "Order: " << ord->getIDhd()
                             << " | Date: " << ord->getDate()
                             << " | Status: " << ord->getStatus()
                             << " | Total: " << ord->getTotalAmount() << " VND" << endl;
                        found = true;
                        if (ord->getStatus() == "Pending" || ord->getIDhd() == string("Waiting for confirmation")) {
                            pending.Push_back(ord);
                        }
                    }
                }
                if (!found) {
                    cout << "No orders found.\n";
                    break;
                }

                // Allow cancellation of pending orders
                    if (!pending.Empty()) {
                    cout << "\nYou have " << pending.getSize() << " pending order(s) that can be cancelled before confirmation." << endl;
                    for (int i = 0; i < pending.getSize(); ++i) {
                        Order* p = pending[i];
                        cout << "[" << i+1 << "] " << "Date: " << p->getDate() << " | Total: " << p->getTotalAmount() << " VND" << endl;
                    }
                    cout << "Enter pending index to cancel (0 to skip): ";
                    int pick = 0; cin >> pick; cin.ignore();
                    if (pick > 0 && pick <= pending.getSize()) {
                        Order* toCancel = pending[pick-1];
                        cout << "Are you sure you want to cancel this order? (y/n): "; char c; cin >> c; cin.ignore();
                        if (c == 'y' || c == 'Y') {
                            // restore product quantities
                            for (int k = 0; k < toCancel->getDetails().getSize(); ++k) {
                                OrderDetail* od = toCancel->getDetails()[k];
                                Product* prod = od->getProduct();
                                if (prod) {
                                    prod->setSLT(prod->getQuantity() + od->getQuantity());
                                }
                            }
                            // persist product changes
                            productDao.saveData();
                            // remove order
                            if (orderDao.removeByPointer(toCancel)) {
                                cout << "Order cancelled and removed successfully." << endl;
                            } else {
                                cout << "Failed to remove order." << endl;
                            }
                        } else {
                            cout << "Cancellation aborted." << endl;
                        }
                    }
                }
                break;
            }
            case 4:
                CreateOrder(orderDao, customerDao, productDao);
                break;
            case 5: {
                CustomerEdit(session.getCurrentCustomer(), customerDao);
                break;
            }
            case 6:
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
    // enable UTF-8 console where supported
    enableUtf8Console();
    SessionManager& session = SessionManager::getInstance();
    // load all data from daos
    session.loadAllData();
    cout << "===> SHOE STORE MANAGEMENT APPLICATION <===" << endl;
    cout << "Customers: " << session.getCustomerDAO()->getDataCache().getSize() << endl;
    cout << "Staff: " << session.getStaffDAO()->getDataCache().getSize() << endl;
    cout << "Products: " << session.getProductDAO()->getDataCache().getSize() << endl;
    cout << "Orders: " << session.getOrderDAO()->getDataCache().getSize() << endl;

    int userType;
    do {
        cout << "\n=== MAIN MENU ===\n";
    cout << "1. Login\n";
    cout << "2. Register (customer)\n";
    cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> userType;
        cin.ignore();

        if (userType == 1) {
            int loginResult = Login(*session.getCustomerDAO(), *session.getStaffDAO(), *session.getAccountDAO());
            if (loginResult == 1) {
                StaffMenu(*session.getCustomerDAO(), *session.getStaffDAO(), *session.getProductDAO(), *session.getOrderDAO(), *session.getAccountDAO(), *session.getCategoryDAO(), *session.getSupplierDAO());
            } else if (loginResult == 2) {
                CustomerMenu(*session.getProductDAO(), *session.getOrderDAO(), *session.getCustomerDAO(), *session.getAccountDAO());
            }
        } else if (userType == 2) {
            Register(*session.getCustomerDAO(), *session.getAccountDAO());
        } else if (userType == 0) {
            cout << "Thank you for using our system!\n";
        } else {
            cout << "Invalid choice!\n";
        }
    } while (userType != 0);
}
