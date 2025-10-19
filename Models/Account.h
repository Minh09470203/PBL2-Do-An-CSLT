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
public:
    string username;
    string password;
    string userType; // "staff", "customer"
    string userID;   // ID user
};

// Declarations free functions
// Forward declare DAOs used in function prototypes
class AccountDAO;
class CustomerDAO;
class StaffDAO;

void ChangePassword(AccountDAO &accountDAO);
int Login(CustomerDAO &customerDAO, StaffDAO &staffDAO, AccountDAO &accountDAO);

#endif // ACCOUNT_H_INCLUDED