#include "CustomerDAO.h"

#include "CustomerDAO.h"
#include "../Models/Customer.h"

bool CustomerDAO::create(const string &id, Customer* entity) {
    dataCache.Push_back(entity);
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "Cannot open " << filename << endl;
        return false;
    }
    file << entity->getID() << "|"
         << entity->getName() << "|"
         << entity->getPhone() << "|"
         << entity->getAddress() << "|"
         << entity->getEmail() << "\n";
    file.close();
    return true;
}

Customer* CustomerDAO::read(const string &id) {
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i]->getID() == id) {
            // Found the customer
            return dataCache[i];
        }
    }
    return nullptr;
}

bool CustomerDAO::update(const string &id, Customer* entity) {
    return saveData();
}

bool CustomerDAO::remove(const string &id) {
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i]->getID() == id) {
            // Found the customer to remove
            delete dataCache[i];
            for (int j = i; j < dataCache.getSize() - 1; ++j) {
                dataCache[j] = dataCache[j + 1];
            }
            dataCache.Pop_back();
            return saveData();
        }
    }
    return true;
}

bool CustomerDAO::loadData() {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, phone, address, email;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, phone, '|');
        getline(ss, address, '|');
        getline(ss, email);
        Customer *c = new Customer(id, name, phone, address, email);
        dataCache.Push_back(c);
    }
    file.close();
    return true;
}

bool CustomerDAO::saveData() {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open " << filename << endl;
        return false;
    }

        for (int i = 0; i < dataCache.getSize(); ++i) {
            file << dataCache[i]->getID() << "|"
                 << dataCache[i]->getName() << "|"
                 << dataCache[i]->getPhone() << "|"
                 << dataCache[i]->getAddress() << "|"
                 << dataCache[i]->getEmail() << "\n";
        }
        file.close();
    return true;
}
