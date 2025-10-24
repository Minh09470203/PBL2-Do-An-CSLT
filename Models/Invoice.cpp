#include "Invoice.h"
#include "../DAO/InvoiceDAO.h"
#include "../DAO/CustomerDAO.h"
#include "../DAO/StaffDAO.h"
#include "../DAO/ProductDAO.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Product.h"

using namespace std;

// Invoice member definitions
string Invoice::getIDhd() { return ID_HD; }
string Invoice::getDate() { return Date; }
Customer* Invoice::getCustomer() { return kh; }
Staff* Invoice::getStaff() { return nv; }
MyVector<OrderDetail*>& Invoice::getDetails() { return details; }
unsigned long Invoice::getTotalAmount() { return TotalAmount; }

void Invoice::setInfo(string id, string date, Customer *customer, Staff *staff) {
    this->ID_HD = id;
    this->Date = date;
    this->kh = customer;
    this->nv = staff;
}

void Invoice::calculateTotal() {
    TotalAmount = 0;
    for(int i = 0; i < details.getSize(); i++) {
        TotalAmount += details[i]->Subtotal();
    }
}

void Invoice::PrintHD() {
    cout << "\nInvoice: " << ID_HD << "\nDate: " << Date
         << "\nCustomer: " << kh->getName() << "\nStaff: " << nv->getNamenv()
         << "\n--- Order detail ---\n";
    for(int i = 0; i < details.getSize(); i++) {
        cout << *details[i] << endl;
    }
    cout << "Total amount: " << TotalAmount << " VND" << endl;
}

void Invoice::addDetail(OrderDetail* od) {
    details.Push_back(od);
}

// Free functions
void CreateInvoice(InvoiceDAO &invoiceDAO, CustomerDAO &customerDAO,
                   StaffDAO &staffDAO, ProductDAO &productDAO)
{
    string invID, date, custID, staffID;
    cout << "Enter Invoice ID: ";
    getline(cin, invID);
    cout << "Enter Date (dd/mm/yyyy): ";
    getline(cin, date);

    if (invoiceDAO.read(invID) != nullptr) {
        cout << "Invoice ID already exists!" << endl;
        return;
    }

    cout << "Enter Customer ID: ";
    getline(cin, custID);
    Customer* cust = customerDAO.read(custID);
    if (!cust) {
        cout << "Customer ID not found. Aborting invoice creation.\n";
        return;
    }

    cout << "Enter Staff ID: ";
    getline(cin, staffID);
    Staff* staff = staffDAO.read(staffID);
    if (!staff) {
        cout << "Staff ID not found. Aborting invoice creation.\n";
        return;
    }

    // Tạo invoice mới
    Invoice* inv = new Invoice();
    inv->setInfo(invID, date, cust, staff);

    int itemCount;
    cout << "Enter number of products in invoice: ";
    cin >> itemCount;
    cin.ignore();

    for (int i = 0; i < itemCount; ++i) {
        string pid;
        int qty;
        cout << "\nProduct #" << i + 1 << "\n";
        cout << "  Enter Product ID: ";
        getline(cin, pid);

        Product* prod = productDAO.read(pid);
        if (!prod) {
            cout << "  Product ID not found. Please re-enter this item.\n";
            --i;
            continue;
        }

        cout << "  Enter Quantity: ";
        cin >> qty;
        cin.ignore();
        
        if (qty <= 0 || qty > prod->getQuantity()) {
            cout << "  Invalid quantity (available: "
                 << prod->getQuantity() << "). Please re-enter this item.\n";
            --i;
            continue;
        }

        // Tạo order detail
        OrderDetail* od = new OrderDetail(prod, qty, prod->getPrice());
        inv->addDetail(od);

        // Update product quantity
        prod->setSLT(prod->getQuantity() - qty);
    }

    // Calculate total
    inv->calculateTotal();

    // Lưu invoice bằng DAO
    if (invoiceDAO.create(invID, inv)) {
        cout << "\nInvoice " << inv->getIDhd() << " created successfully.\n";
        
        // Update product quantities trong productDAO
        productDAO.saveAll();
    } else {
        cout << "Failed to save invoice." << endl;
        delete inv;  // Cleanup nếu fail
    }
}

void printInvoiceList(InvoiceDAO &invoiceDAO) {
    MyVector<Invoice*>& invoices = invoiceDAO.getDataCache();
    cout << left << setw(15) << "Invoice ID"
         << setw(12) << "Date"
         << setw(18) << "Customer"
         << setw(18) << "Staff"
         << setw(12) << "Total Amount" << endl;
    cout << string(82, '-') << endl;
    for (int i = 0; i < invoices.getSize(); i++) {
        cout << left << setw(15) << invoices[i]->getIDhd()
             << setw(12) << invoices[i]->getDate()
             << setw(18) << invoices[i]->getCustomer()->getName()
             << setw(18) << invoices[i]->getStaff()->getNamenv()
             << setw(12) << invoices[i]->getTotalAmount() << "VND" << endl;
        cout << string(82, '-') << endl;
    }
}