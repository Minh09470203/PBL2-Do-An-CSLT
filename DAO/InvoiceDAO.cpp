#include "InvoiceDAO.h"
#include "CustomerDAO.h"
#include "StaffDAO.h"
#include "ProductDAO.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool InvoiceDAO::create(const string &id, Invoice* entity) {
    dataCache.Push_back(entity);
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "Cannot open " << filename << endl;
        return false;
    }
    // write header (new order: ID|Date|Status|CustomerID|StaffID|Total)
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

Invoice* InvoiceDAO::read(const string &id) {
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i]->getIDhd() == id) return dataCache[i];
    }
    return nullptr;
}

bool InvoiceDAO::update(const string &id, Invoice* entity) {
    return saveData();
}

bool InvoiceDAO::remove(const string &id) {
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

bool InvoiceDAO::loadData(CustomerDAO &customerDAO, StaffDAO &staffDAO, ProductDAO &productDAO) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file " << filename << endl;
        return false;
    }

    string line;
    Invoice* currentInvoice = nullptr;

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
            Staff* staff = nullptr;

            if (cust) {
                if (staffID != "Waiting for confirmation") {
                    staff = staffDAO.read(staffID);
                    if (!staff) {
                        cout << "Warning: Staff " << staffID << " not found for invoice " << invID << endl;
                    }
                }

                unsigned long total = 0;
                try {
                    if (!totalStr.empty()) total = stoul(totalStr);
                } catch (const std::exception &e) {
                    cout << "Warning: invalid total '" << totalStr << "' for invoice " << invID << ". Defaulting to 0." << endl;
                    total = 0;
                }

                currentInvoice = new Invoice(invID, date, cust, staff, total, status);
                dataCache.Push_back(currentInvoice);
            } else {
                // create invoice with nullptr refs to avoid crash but still store
                unsigned long total = 0;
                try {
                    if (!totalStr.empty()) total = stoul(totalStr);
                } catch (const std::exception &e) {
                    cout << "Warning: invalid total '" << totalStr << "' for invoice " << invID << ". Defaulting to 0." << endl;
                    total = 0;
                }
                currentInvoice = new Invoice(invID, date, nullptr, nullptr, total, status);
                dataCache.Push_back(currentInvoice);
                cout << "Warning: Customer " << custID << " not found for invoice " << invID << endl;
            }
        } else {
            if (currentInvoice != nullptr) {
                stringstream dss(line.substr(1));
                string prodID, qtyStr, priceStr;
                getline(dss, prodID, '|');
                getline(dss, qtyStr, '|');
                getline(dss, priceStr);

                Product* prod = productDAO.read(prodID);

                if (prod) {
                    OrderDetail* od = new OrderDetail(prod, stoi(qtyStr), stoul(priceStr));
                    currentInvoice->addDetail(od);
                } else {
                    cout << "Warning: Product " << prodID << " not found for invoice details" << endl;
                }
            }
        }
    }

    file.close();
    return true;
}

bool InvoiceDAO::loadData() {
    // Not used but required by IDAO
    return false;
}

bool InvoiceDAO::saveData() {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open " << filename << endl;
        return false;
    }

    for (int i = 0; i < dataCache.getSize(); ++i) {
        Invoice* inv = dataCache[i];
           file << inv->getIDhd() << "|" << inv->getDate() << "|"
               << (inv->getStatus().empty() ? string("") : inv->getStatus()) << "|"
               << (inv->getCustomer() ? inv->getCustomer()->getID() : string("")) << "|"
               << (inv->getStaff() ? inv->getStaff()->getIDnv() : string("Waiting for confirmation")) << "|"
               << inv->getTotalAmount() << "\n";
        for (int k = 0; k < inv->getDetails().getSize(); ++k) {
            OrderDetail* od = inv->getDetails()[k];
            file << "-" << od->getProduct()->getIDsp() << "|" << od->getQuantity() << "|" << od->getUnitPrice() << "\n";
        }
    }

    file.close();
    return true;
}

bool InvoiceDAO::removeByPointer(Invoice* inv) {
    if (!inv) return false;
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i] == inv) {
            delete dataCache[i];
            for (int j = i; j < dataCache.getSize() - 1; ++j) dataCache[j] = dataCache[j+1];
            dataCache.Pop_back();
            return saveData();
        }
    }
    return false;
}
