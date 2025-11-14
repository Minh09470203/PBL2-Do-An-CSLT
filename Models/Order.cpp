#include "Order.h"
#include "../DAO/OrderDAO.h"
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
// Order member definitions
string Order::getIDhd() { return ID_HD; }
string Order::getDate() { return Date; }
Customer* Order::getCustomer() { return kh; }
Staff* Order::getStaff() { return nv; }
void Order::setStaff(Staff *nv) { this->nv = nv; }
MyVector<OrderDetail*>& Order::getDetails() { return details; }
unsigned long Order::getTotalAmount() { return TotalAmount; }
string Order::getStatus() { return orderStatus; }
void Order::setStatus(const string &status) { this->orderStatus = status; }

void Order::setInfo(string id, string date, Customer *customer) {
    this->ID_HD = id;
    this->Date = date;
    this->kh = customer;
}

void Order::calculateTotal() {
    TotalAmount = 0;
    for(int i = 0; i < details.getSize(); i++) {
        TotalAmount += details[i]->Subtotal();
    }
}

void Order::PrintHD() {
    cout << "\nOrder: " << ID_HD << "\nDate: " << Date
         << "\nCustomer: " << (kh ? kh->getName() : string("<unknown>"))
         << "\nStatus: " << (orderStatus.empty() ? string("<unknown>") : orderStatus)
         << "\nStaff: " << (nv ? nv->getName() : string("Waiting for confirmation"))
         << "\n--- Order detail ---\n";
    for(int i = 0; i < details.getSize(); i++) {
        cout << *details[i] << endl;
    }
    cout << "Total amount: " << TotalAmount << " VND" << endl;
}

void Order::addDetail(OrderDetail* od) {
    details.Push_back(od);
}

// Free helper: process pending order requests and allow a staff to confirm and assign an order ID
void ProcessOrderRequests(OrderDAO &orderDao, Staff *currentStaff) {
    bool any = false;
    cout << "\nPending order requests:\n";
    MyVector<Order*> pending;
    for (int i = 0; i < orderDao.getDataCache().getSize(); ++i) {
        Order* ord = orderDao.getDataCache()[i];
        if (ord->getIDhd() == string("Waiting for confirmation") || ord->getStatus() == string("Pending")) {
            pending.Push_back(ord);
            cout << "[" << pending.getSize() << "] "
                 << "Customer: " << (ord->getCustomer() ? ord->getCustomer()->getName() : string("<unknown>"))
                 << " | Date: " << ord->getDate() << " | Total: " << ord->getTotalAmount() << " VND" << endl;
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
        Order* ord = pending[pickIdx - 1];
    string newID;
    cout << "Enter new Order ID to assign: ";
        getline(cin, newID);
        if (newID.empty()) {
            cout << "Cancelled - no ID entered.\n";
            return;
        }
        if (orderDao.read(newID) != nullptr) {
            cout << "Order ID already exists. Aborting.\n";
            return;
        }
        ord->setInfo(newID, ord->getDate(), ord->getCustomer());
        // assign confirming staff
        ord->setStaff(currentStaff);
        ord->setStatus("Confirmed");
        if (orderDao.saveData()) {
            cout << "Order " << newID << " confirmed." << "\n";
        } else {
            cout << "Failed to save order confirmation." << "\n";
        }
    } else {
        cout << "Cancelled.\n";
    }
}

// Free helper: advance order status through the sequence Pending -> Confirmed -> Packing -> Shipping -> Completed
void HandleOrderStatusUpdate(OrderDAO &orderDao) {
    string ordID;
    cout << "Enter order ID to update status: ";
    getline(cin, ordID);
    Order* ord = orderDao.read(ordID);
    if (!ord) {
        cout << "Order not found.\n";
        return;
    }
    string curr = ord->getStatus();
    string next;
    if (curr == "Confirmed") next = "Packing";
    else if (curr == "Packing") next = "Shipping";
    else if (curr == "Shipping") next = "Completed";
    else if (curr == "Pending") next = "Confirmed"; // allow staff/manager to confirm pending
    else if (curr == "Completed") next = "";
    else next = "Confirmed";

    if (next.empty()) {
        cout << "Order is already Completed. No further updates possible.\n";
        return;
    }

    cout << "Current status: " << curr << ", next status: " << next << ". Proceed? (y/n): ";
    char ch; cin >> ch; cin.ignore();
    if (ch == 'y' || ch == 'Y') {
        ord->setStatus(next);
    if (orderDao.saveData()) {
            cout << "Order " << ordID << " status updated to " << next << "\n";
        } else {
            cout << "Failed to save status update.\n";
        }
    } else {
        cout << "Cancelled.\n";
    }
}

// Free functions
void CreateOrder(OrderDAO &orderDAO, CustomerDAO &customerDAO, ProductDAO &productDAO)
{
    SessionManager &session = SessionManager::getInstance();
    Customer* sessionCust = session.getCurrentCustomer();

    string ordID, date;
    cout << "Enter Date (dd/mm/yyyy): ";
    getline(cin, date);

    if (orderDAO.read(ordID) != nullptr) {
        cout << "Order ID already exists!" << endl;
        return;
    }

    Customer* cust = nullptr;

    if (sessionCust) {
        // Customer-initiated order: use logged-in customer
        cust = sessionCust;
        ordID = string("Waiting for confirmation"); // placeholder until confirmed
        cout << "Creating order as customer: " << cust->getName() << " (will be pending)\n";
    } else {
        cout << "Only customers can create orders. Please log in as a customer.\n";
        return;
    }

    // Create order
    Order* ord = new Order();
    ord->setInfo(ordID, date, cust);
    ord->setStatus("Pending");

    int itemCount;
    cout << "Enter number of products in order: ";
    cin >> itemCount;
    if (itemCount <= 0) {
        cout << "Invalid number." << endl;
        delete ord;
        return;
    }
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
    ord->addDetail(od);
        prod->setSLT(prod->getQuantity() - qty);
    }

    ord->calculateTotal();

    if (orderDAO.create(ordID, ord)) {
        cout << "\nOrder " << ord->getIDhd() << " created successfully and is pending confirmation." << "\n";
        productDAO.saveData();
    } else {
        cout << "Failed to save order." << endl;
        delete ord;
    }
}

void printOrderList(OrderDAO &orderDAO) {
    MyVector<Order*>& orders = orderDAO.getDataCache();
    cout << left << setw(15) << "Order ID"
         << setw(12) << "Date"
         << setw(12) << "Status"
         << setw(18) << "Customer"
         << setw(12) << "Total Amount" << endl;
    cout << string(78, '-') << endl;
    for (int i = 0; i < orders.getSize(); i++) {
       cout << left << setw(15) << orders[i]->getIDhd()
           << setw(12) << orders[i]->getDate()
           << setw(12) << orders[i]->getStatus()
           << setw(18) << (orders[i]->getCustomer() ? orders[i]->getCustomer()->getName() : string("<unknown>"))
           << setw(12) << orders[i]->getTotalAmount() << "VND" << endl;
        cout << string(78, '-') << endl;
    }
}
