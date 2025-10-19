#ifndef INVOICE_H_INCLUDED
#define INVOICE_H_INCLUDED
#include "Customer.h"
#include "Staff.h"
#include "Product.h"
#include "OrderDetail.h"
#include "../DataStructure/MyVector.h"

class Invoice {
private:
    string ID_HD;
    string Date;
    Customer *kh;
    Staff *nv;
    unsigned long TotalAmount;
    MyVector<OrderDetail*> details;
public:
    Invoice(
            string ID_HD = "",
            string Date = "",
            Customer *kh = nullptr,
            Staff *nv = nullptr,
            unsigned long TotalAmount = 0
    )
    : ID_HD(ID_HD), Date(Date), kh(kh), nv(nv), TotalAmount(TotalAmount) {}

    ~Invoice() {
        for (int i = 0; i < details.getSize(); i++) {
            delete details[i];
        }
    }

    string getIDhd();
    string getDate();
    Customer *getCustomer();
    Staff *getStaff();
    MyVector<OrderDetail*>& getDetails();
    unsigned long getTotalAmount();
    void setInfo(string id, string date, Customer *customer, Staff *staff);

    void calculateTotal();

    void PrintHD();

    void addDetail(OrderDetail* od);
};

// Declarations invoice functions
// Forward declare DAO types used in prototypes
class InvoiceDAO;
class CustomerDAO;
class StaffDAO;
class ProductDAO;

void CreateInvoice(InvoiceDAO &invoiceDAO, CustomerDAO &customerDAO, 
                   StaffDAO &staffDAO, ProductDAO &productDAO);

void printInvoiceList(InvoiceDAO &invoiceDAO);

#endif // INVOICE_H_INCLUDED
