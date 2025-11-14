#ifndef SESSIONMANAGER_H_INCLUDED
#define SESSIONMANAGER_H_INCLUDED
#include <iostream>
#include <string>
#include "../Models/Customer.h"
#include "../Models/Staff.h"
#include "../DAO/CustomerDAO.h"
#include "../DAO/StaffDAO.h"
#include "../DAO/ProductDAO.h"
#include "../DAO/OrderDAO.h"
#include "../DAO/AccountDAO.h"
#include "../DAO/CategoryDAO.h"
#include "../DAO/SupplierDAO.h"

class SessionManager {
private:
    static SessionManager* instance;
    Customer* currentCustomer;
    Staff* currentStaff;
    string currentUsername;
    
    // Data Access Objects
    CustomerDAO* customerDAO;
    StaffDAO* staffDAO;
    ProductDAO* productDAO;
    OrderDAO* orderDAO;
    AccountDAO* accountDAO;
    CategoryDAO* categoryDAO;
    SupplierDAO* supplierDAO;
    // Private constructor - defined in .cpp
    SessionManager();
    
    // Prevent copy constructor and assignment operator
    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;

public:
    static SessionManager& getInstance();
    // Getter functions
    Customer* getCurrentCustomer();
    Staff* getCurrentStaff();
    string getCurrentUsername();
    CustomerDAO* getCustomerDAO();
    StaffDAO* getStaffDAO();
    ProductDAO* getProductDAO();
    OrderDAO* getOrderDAO();
    AccountDAO* getAccountDAO();
    CategoryDAO* getCategoryDAO();
    SupplierDAO* getSupplierDAO();

    // Setter functions
    void setCurrentCustomer(Customer* customer);
    void setCurrentStaff(Staff* staff);
    void setCurrentUsername(const string& username);
    // Logout function - reset all
    void logout();
    bool isLoggedIn();
    bool isStaff();
    bool isCustomer();
    // Load all DAOs
    bool loadAllData();
    ~SessionManager();
};

// static member is defined in SessionManager.cpp

#endif // SESSIONMANAGER_H_INCLUDED