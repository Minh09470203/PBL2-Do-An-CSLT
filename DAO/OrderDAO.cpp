#include "OrderDAO.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool OrderDAO::create(const string &id, Order* entity) {
    dataCache.Push_back(entity);
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "Cannot open " << filename << endl;
        return false;
    }
    // write header (new order: ID|Date|Status|CustomerID|Total)
    file << entity->getIDhd() << "|" << entity->getDate() << "|"
        << (entity->getStatus().empty() ? string("") : entity->getStatus()) << "|"
        << (entity->getCustomer() ? entity->getCustomer()->getID() : string("")) << "|"
        << (entity->getStaff() ? entity->getStaff()->getIDnv() : string("Waiting for confirmation")) << "|"
        << entity->getTotalAmount() << "\n";
    // write details
    for (int i = 0; i < entity->getDetails().getSize(); ++i) {
        OrderDetail* od = entity->getDetails()[i];
        file << "-" << od->getProduct()->getIDsp() << "|" << od->getQuantity() << "|" << od->getUnitPrice() << "\n";
    }
    file.close();
    return true;
}

Order* OrderDAO::read(const string &id) {
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i]->getIDhd() == id) return dataCache[i];
    }
    return nullptr;
}

bool OrderDAO::update(const string &id, Order* entity) {
    return saveData();
}

bool OrderDAO::remove(const string &id) {
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i]->getIDhd() == id) {
            delete dataCache[i];
            for (int j = i; j < dataCache.getSize() - 1; ++j) dataCache[j] = dataCache[j+1];
            dataCache.Pop_back();
            return saveData();
        }
    }
    return true;
}

bool OrderDAO::loadData(CustomerDAO &customerDAO, StaffDAO &staffDAO, ProductDAO &productDAO) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file " << filename << endl;
        return false;
    }

    string line;
    Order* currentOrder = nullptr;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line[0] != '-') {
            stringstream ss(line);
            string invID, date, status, custID, staffID, totalStr;
            getline(ss, invID, '|');
            getline(ss, date, '|');
            // status is now the 3rd field
            getline(ss, status, '|');
            getline(ss, custID, '|');
            getline(ss, staffID, '|');
            getline(ss, totalStr);

            Customer* cust = customerDAO.read(custID);
            unsigned long total = 0;
            try { if (!totalStr.empty()) total = stoul(totalStr); } catch (...) { total = 0; }
            Staff* staffPtr = nullptr;
            if (!staffID.empty() && staffID != string("Waiting for confirmation")) {
                staffPtr = staffDAO.read(staffID);
            }

            if (cust) {
                currentOrder = new Order(invID, date, cust, staffPtr, total, status);
                dataCache.Push_back(currentOrder);
            } else {
                currentOrder = new Order(invID, date, nullptr, staffPtr, total, status);
                dataCache.Push_back(currentOrder);
                cout << "Warning: Customer " << custID << " not found for order " << invID << endl;
            }
        } else {
            if (currentOrder != nullptr) {
                stringstream dss(line.substr(1));
                string prodID, qtyStr, priceStr;
                getline(dss, prodID, '|');
                getline(dss, qtyStr, '|');
                getline(dss, priceStr);

                Product* prod = productDAO.read(prodID);

                if (prod) {
                    OrderDetail* od = new OrderDetail(prod, stoi(qtyStr), stoul(priceStr));
                    currentOrder->addDetail(od);
                } else {
                    cout << "Warning: Product " << prodID << " not found for order details" << endl;
                }
            }
        }
    }

    file.close();
    return true;
}

bool OrderDAO::loadData() {
    // Not used but required by IDAO
    return false;
}

bool OrderDAO::saveData() {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file " << filename << endl;
        return false;
    }

    for (int i = 0; i < dataCache.getSize(); ++i) {
        Order* ord = dataCache[i];
           file << ord->getIDhd() << "|" << ord->getDate() << "|"
               << (ord->getStatus().empty() ? string("") : ord->getStatus()) << "|"
               << (ord->getCustomer() ? ord->getCustomer()->getID() : string("")) << "|"
               << (ord->getStaff() ? ord->getStaff()->getIDnv() : string("Waiting for confirmation")) << "|"
               << ord->getTotalAmount() << "\n";
        for (int k = 0; k < ord->getDetails().getSize(); ++k) {
            OrderDetail* od = ord->getDetails()[k];
            file << "-" << od->getProduct()->getIDsp() << "|" << od->getQuantity() << "|" << od->getUnitPrice() << "\n";
        }
    }

    file.close();
    return true;
}

bool OrderDAO::removeByPointer(Order* ord) {
    if (!ord) return false;
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i] == ord) {
            delete dataCache[i];
            for (int j = i; j < dataCache.getSize() - 1; ++j) dataCache[j] = dataCache[j+1];
            dataCache.Pop_back();
            return saveData();
        }
    }
    return false;
}
