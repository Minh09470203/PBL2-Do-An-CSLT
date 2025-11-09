#include "Invoice.h"
#include "../DAO/InvoiceDAO.h"
#include "../DAO/CustomerDAO.h"
#include "../DAO/StaffDAO.h"
#include "../DAO/ProductDAO.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Product.h"
#include <iomanip>
#include "../AppSession/SessionManager.h"

using namespace std;
// use the literal "Waiting for confirmation" for pending values (keep code simple)
// Invoice member definitions
string Invoice::getIDhd() { return ID_HD; }
string Invoice::getDate() { return Date; }
Customer* Invoice::getCustomer() { return kh; }
Staff* Invoice::getStaff() { return nv; }
MyVector<OrderDetail*>& Invoice::getDetails() { return details; }
unsigned long Invoice::getTotalAmount() { return TotalAmount; }
string Invoice::getStatus() { return Status; }
void Invoice::setStatus(const string &status) { this->Status = status; }

void Invoice::setInfo(string id, string date, Customer *customer, Staff *staff) {
    this->ID_HD = id;
    this->Date = date;
    this->kh = customer;
    this->nv = staff;
}

void Invoice::setStaff(Staff *staff) {
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
         << "\nCustomer: " << (kh ? kh->getName() : string("<unknown>"))
         << "\nStaff: " << (nv ? nv->getNamenv() : string("Waiting for confirmation"))
         << "\nStatus: " << (Status.empty() ? string("<unknown>") : Status)
         << "\n--- Order detail ---\n";
    for(int i = 0; i < details.getSize(); i++) {
        cout << *details[i] << endl;
    }
    cout << "Total amount: " << TotalAmount << " VND" << endl;
}

void Invoice::addDetail(OrderDetail* od) {
    details.Push_back(od);
}

// Free helper: process pending order requests and allow a staff to confirm and assign an invoice ID
void ProcessOrderRequests(InvoiceDAO &invoiceDao, Staff *currentStaff) {
    bool any = false;
    cout << "\nPending invoice requests:\n";
    MyVector<Invoice*> pending;
    for (int i = 0; i < invoiceDao.getDataCache().getSize(); ++i) {
        Invoice* inv = invoiceDao.getDataCache()[i];
        if (inv->getIDhd() == string("Waiting for confirmation") || inv->getStaff() == nullptr) {
            pending.Push_back(inv);
            cout << "[" << pending.getSize() << "] "
                 << "Customer: " << (inv->getCustomer() ? inv->getCustomer()->getName() : string("<unknown>"))
                 << " | Date: " << inv->getDate() << " | Total: " << inv->getTotalAmount() << " VND" << endl;
            any = true;
        }
    }
    if (!any) {
        cout << "No pending requests.\n";
        return;
    }

    int pickIdx = 0;
    cout << "Enter pending index to confirm (0 to cancel): ";
    cin >> pickIdx;
    cin.ignore();
    if (pickIdx > 0 && pickIdx <= pending.getSize()) {
        Invoice* inv = pending[pickIdx - 1];
        string newID;
        cout << "Enter new Invoice ID to assign: ";
        getline(cin, newID);
        if (newID.empty()) {
            cout << "Cancelled - no ID entered.\n";
            return;
        }
        if (invoiceDao.read(newID) != nullptr) {
            cout << "Invoice ID already exists. Aborting.\n";
            return;
        }
        inv->setInfo(newID, inv->getDate(), inv->getCustomer(), currentStaff);
        inv->setStatus("Confirmed");
        if (invoiceDao.saveData()) {
            cout << "Invoice " << newID << " confirmed and assigned to you.\n";
        } else {
            cout << "Failed to save invoice confirmation.\n";
        }
    } else {
        cout << "Cancelled.\n";
    }
}

// Free helper: advance invoice status through the sequence Pending -> Confirmed -> Shipping -> Completed
void HandleOrderStatusUpdate(InvoiceDAO &invoiceDao) {
    string invID;
    cout << "Enter invoice ID to update status: ";
    getline(cin, invID);
    Invoice* inv = invoiceDao.read(invID);
    if (!inv) {
        cout << "Invoice not found.\n";
        return;
    }
    string curr = inv->getStatus();
    string next;
    if (curr == "Confirmed") next = "Shipping";
    else if (curr == "Shipping") next = "Completed";
    else if (curr == "Pending") next = "Confirmed"; // allow staff/manager to confirm pending
    else if (curr == "Completed") next = "";
    else next = "Confirmed";

    if (next.empty()) {
        cout << "Invoice is already Completed. No further updates possible.\n";
        return;
    }

    cout << "Current status: " << curr << ", next status: " << next << ". Proceed? (y/n): ";
    char ch; cin >> ch; cin.ignore();
    if (ch == 'y' || ch == 'Y') {
        inv->setStatus(next);
        if (invoiceDao.saveData()) {
            cout << "Invoice " << invID << " status updated to " << next << "\n";
        } else {
            cout << "Failed to save status update.\n";
        }
    } else {
        cout << "Cancelled.\n";
    }
}

// Free functions
void CreateInvoice(InvoiceDAO &invoiceDAO, CustomerDAO &customerDAO,
                   StaffDAO &staffDAO, ProductDAO &productDAO)
{
    SessionManager &session = SessionManager::getInstance();
    Customer* sessionCust = session.getCurrentCustomer();
    Staff* sessionStaff = session.getCurrentStaff();

    string invID, date;
    cout << "Enter Date (dd/mm/yyyy): ";
    getline(cin, date);

    if (invoiceDAO.read(invID) != nullptr) {
        cout << "Invoice ID already exists!" << endl;
        return;
    }

    Customer* cust = nullptr;
    Staff* staff = nullptr;

    if (sessionCust) {
        // Customer-initiated order: use logged-in customer, staff remains nullptr (pending)
        cust = sessionCust;
        staff = nullptr;
        invID = string("Waiting for confirmation"); // placeholder until staff assigns real ID
        cout << "Creating invoice as customer: " << cust->getName() << " (will be pending)\n";
        // set status to Pending for customer-created invoices
        
    } else if (sessionStaff) {
        // Staff-initiated: ask for customer ID and optional staff ID
        string custID, staffID;
        cout << "Enter Customer ID: ";
        getline(cin, custID);
        cust = customerDAO.read(custID);
        if (!cust) {
            cout << "Customer ID not found. Aborting invoice creation.\n";
            return;
        }

        cout << "Enter Staff ID (leave blank to create request pending confirmation): ";
        getline(cin, staffID);
        if (!staffID.empty()) {
            staff = staffDAO.read(staffID);
            if (!staff) {
                cout << "Staff ID not found. Aborting invoice creation.\n";
                return;
            }
        }
        cout << "Enter Invoice ID: ";
        getline(cin, invID);
        if (invoiceDAO.read(invID) != nullptr) {
            cout << "Invoice ID already exists!" << endl;
            return;
        }
        } else {
        cout << "No user session available. Cannot create invoice.\n";
        return;
        }

    // Create invoice
    Invoice* inv = new Invoice();
    inv->setInfo(invID, date, cust, staff);
    // set status depending on who created the invoice
    if (cust && staff == nullptr) {
        inv->setStatus("Pending");
    } else if (staff != nullptr) {
        // If a staff creates the invoice, mark it Completed immediately
        inv->setStatus("Completed");
    } else {
        inv->setStatus("Pending");
    }

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
            cout << "  Product ID not found. Please re-enter this item." << endl;
            --i;
            continue;
        }

        cout << "  Enter Quantity: ";
        cin >> qty;
        cin.ignore();

        if (qty <= 0 || qty > prod->getQuantity()) {
            cout << "  Invalid quantity (available: " << prod->getQuantity() << "). Please re-enter this item." << endl;
            --i;
            continue;
        }

        OrderDetail* od = new OrderDetail(prod, qty, prod->getPrice());
        inv->addDetail(od);
        prod->setSLT(prod->getQuantity() - qty);
    }

    inv->calculateTotal();

    if (invoiceDAO.create(invID, inv)) {
        if (inv->getStaff() == nullptr) {
            cout << "\nInvoice " << inv->getIDhd() << " created successfully and is pending confirmation." << "\n";
        } else {
            cout << "\nInvoice " << inv->getIDhd() << " created successfully. Status: " << inv->getStatus() << "\n";
        }
    productDAO.saveData();
    } else {
        cout << "Failed to save invoice." << endl;
        delete inv;
    }
}

void printInvoiceList(InvoiceDAO &invoiceDAO) {
    MyVector<Invoice*>& invoices = invoiceDAO.getDataCache();
    cout << left << setw(15) << "Invoice ID"
         << setw(12) << "Date"
         << setw(12) << "Status"
         << setw(18) << "Customer"
        << setw(18) << "Staff"
         << setw(12) << "Total Amount" << endl;
    cout << string(96, '-') << endl;
    for (int i = 0; i < invoices.getSize(); i++) {
       cout << left << setw(15) << invoices[i]->getIDhd()
           << setw(12) << invoices[i]->getDate()
           << setw(12) << invoices[i]->getStatus()
           << setw(18) << (invoices[i]->getCustomer() ? invoices[i]->getCustomer()->getName() : string("<unknown>"))
           << setw(18) << (invoices[i]->getStaff() ? invoices[i]->getStaff()->getNamenv() : string("Waiting for confirmation"))
           << setw(12) << invoices[i]->getTotalAmount() << "VND" << endl;
        cout << string(96, '-') << endl;
    }
}

// (function CreateInvoiceCustomer removed; unified in CreateInvoice)