#ifndef ACCOUNT_H_INCLUDED
#define ACCOUNT_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "../DataStructure/MyVector.h"
#include "Customer.h"
#include "Staff.h"
using namespace std;
class Account {
private:
    string username;
    string password;
    string userType; // "staff", "customer"
    string userID;   // ID user

public:
    // Getters
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getUserType() const { return userType; }
    string getUserID() const { return userID; }

    // Setters
    void setUsername(const string& u) { this->username = u; }
    void setPassword(const string& p) { this->password = p; }
    void setUserType(const string& t) { this->userType = t; }
    void setUserID(const string& id) { this->userID = id; }

    // Stream I/O
    ostream& output(ostream& os) const;
    friend ostream& operator<<(ostream& os, const Account& a);
};
// Forward declare DAOs used in function prototypes
class AccountDAO;
class CustomerDAO;
class StaffDAO;

void ChangePassword(AccountDAO &accountDAO);
int Login(CustomerDAO &customerDAO, StaffDAO &staffDAO, AccountDAO &accountDAO);
void Register(CustomerDAO &customerDAO, AccountDAO &accountDAO);

#endif // ACCOUNT_H_INCLUDED