#ifndef CUSTOMER_H_INCLUDED
#define CUSTOMER_H_INCLUDED
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "Person.h"

using namespace std;
// Forward declare DAO used in function prototypes to avoid circular include
class CustomerDAO;

class Customer : public Person {
private:
    string Address;
    string Email;
public:
    Customer(
             string ID_KH = "",
             string Name_KH = "",
             string SDT = "",
             string Address = "",
             string Email = ""
    )
    : Person(ID_KH, Name_KH, SDT), Address(Address), Email(Email) {}

    // Override virtual methods for polymorphism
    void display() const override;

    // Keep existing method for compatibility
    void PrintKH();

    string getAddress();
    string getEmail();
    void setEmail(string newEmail);
    void setAddress(string newAddress);
};
// Declarations for functions implemented in Customer.cpp
void printCustomerList(CustomerDAO &customerDAO);
bool isduplicateCustomer(CustomerDAO &customerDAO, const string& id);
void CustomerInsert(CustomerDAO &customerDAO);
void CustomerEdit(Customer* currentUser, CustomerDAO &customerDAO);
void CustomerSearch(CustomerDAO &customerDAO);
#endif // CUSTOMER_H_INCLUDED
