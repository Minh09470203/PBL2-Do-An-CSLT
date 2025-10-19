#include "SessionManager.h"
using namespace std;
// Session member definitions
// Constructor definition (was declared in header)
SessionManager::SessionManager() {
    currentCustomer = nullptr;
    currentStaff = nullptr;
    currentUsername = "";
    // Instantiate DAOs with correct data file names
    accountDAO = new AccountDAO("DataFile/Accounts.txt");
    customerDAO = new CustomerDAO("DataFile/Customer.txt");
    staffDAO = new StaffDAO("DataFile/Staff.txt");
    categoryDAO = new CategoryDAO("DataFile/Category.txt");
    supplierDAO = new SupplierDAO("DataFile/Supplier.txt");
    productDAO = new ProductDAO("DataFile/Product.txt");
    invoiceDAO = new InvoiceDAO("DataFile/Invoice.txt");
}

SessionManager& SessionManager::getInstance() {
    if (instance == nullptr) {
        instance = new SessionManager();
    }
    return *instance;
}
Customer* SessionManager::getCurrentCustomer() { return currentCustomer; }
Staff* SessionManager::getCurrentStaff() { return currentStaff; }
string SessionManager::getCurrentUsername() { return currentUsername; }
CustomerDAO* SessionManager::getCustomerDAO() { return customerDAO; }
StaffDAO* SessionManager::getStaffDAO() { return staffDAO; }
ProductDAO* SessionManager::getProductDAO() { return productDAO; }
InvoiceDAO* SessionManager::getInvoiceDAO() { return invoiceDAO; }
AccountDAO* SessionManager::getAccountDAO() { return accountDAO; }
CategoryDAO* SessionManager::getCategoryDAO() { return categoryDAO; }
SupplierDAO* SessionManager::getSupplierDAO() { return supplierDAO; }

void SessionManager::setCurrentCustomer(Customer* customer) {
    currentCustomer = customer;
    currentStaff = nullptr; // ensure only one type is set
}
void SessionManager::setCurrentStaff(Staff* staff) {
    currentStaff = staff;
    currentCustomer = nullptr; // ensure only one type is set
}
void SessionManager::setCurrentUsername(const string& username) {
    currentUsername = username;
}
void SessionManager::logout() {
    currentCustomer = nullptr;
    currentStaff = nullptr;
    currentUsername = "";
    cout << "Logged out successfully!\n";
}
bool SessionManager::isLoggedIn() {
    return (currentCustomer != nullptr || currentStaff != nullptr);
}
bool SessionManager::isStaff() { return currentStaff != nullptr; }
bool SessionManager::isCustomer() { return currentCustomer != nullptr; }   

bool SessionManager::loadAllData() {
    cout << "Loading data from files..." << endl;
    bool success = true;
    success &= accountDAO->loadAll();
    success &= customerDAO->loadAll();
    success &= staffDAO->loadAll();
    success &= categoryDAO->loadAll();
    success &= supplierDAO->loadAll();
    success &= productDAO->loadAll(*supplierDAO, *categoryDAO);
    success &= invoiceDAO->loadAll(*customerDAO, *staffDAO, *productDAO);
    if (success) {
        cout << "All data loaded successfully!\n" << endl;
    } else {
        cout << "Warning!!! " << endl;
        cout << "Some data failed to load.\n" << endl;
    }
    return success;
}

// Destructor to clean up DAOs
SessionManager::~SessionManager() {
    delete accountDAO;
    delete customerDAO;
    delete staffDAO;
    delete categoryDAO;
    delete supplierDAO;
    delete productDAO;
    delete invoiceDAO;
}
SessionManager* SessionManager::instance = nullptr;
