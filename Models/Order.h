#ifndef ORDER_H_INCLUDED
#define ORDER_H_INCLUDED
#include "Customer.h"
#include "Staff.h"
#include "Product.h"
#include "OrderDetail.h"
#include "../DataStructure/MyVector.h"

class Order {
private:
    string ID_HD;
    string Date;
    Customer *kh;
    Staff *nv;
    unsigned long TotalAmount;
    MyVector<OrderDetail*> details;
    string orderStatus;
public:
    Order(
            string ID_HD = "",
            string Date = "",
            Customer *kh = nullptr,
            Staff *nv = nullptr,
        unsigned long TotalAmount = 0,
        string Status = string("")
    )
    : ID_HD(ID_HD), Date(Date), kh(kh), nv(nv), TotalAmount(TotalAmount), orderStatus(Status) {}

    ~Order() {
        for (int i = 0; i < details.getSize(); i++) {
            delete details[i];
        }
    }

    string getIDhd();
    string getDate();
    Customer *getCustomer();
    Staff *getStaff();
    void setStaff(Staff *nv);
    MyVector<OrderDetail*>& getDetails();
    unsigned long getTotalAmount();
    string getStatus();
    void setStatus(const string &status);
    void setInfo(string id, string date, Customer *customer);
    void advanceStatus();

    void calculateTotal();

    void PrintHD();

    void addDetail(OrderDetail* od);
    // Helpers moved from main: handle pending requests and status updates
    // (now declared as free functions below)
};

// Declarations order functions
// Forward declare DAO types used in prototypes
class OrderDAO;
class CustomerDAO;
class StaffDAO;
class ProductDAO;

void CreateOrder(OrderDAO &orderDAO, CustomerDAO &customerDAO, ProductDAO &productDAO);

// Free helper functions (moved out of class)
void ProcessOrderRequests(OrderDAO &orderDao, Staff *currentStaff);
void HandleOrderStatusUpdate(OrderDAO &orderDao);
void ViewOrder(OrderDAO &orderDao, ProductDAO &productDAO);
void printOrderList(OrderDAO &orderDAO);

#endif // ORDER_H_INCLUDED
