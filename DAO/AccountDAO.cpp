#include "AccountDAO.h"
#include "../Models/Account.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool AccountDAO::create(const string &id, Account* entity) {
    dataCache.Push_back(entity);
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "Cannot open " << filename << endl;
        return false;
    }
    file << entity->getUsername() << "|" << entity->getPassword() << "|" << entity->getUserType() << "|" << entity->getUserID() << "\n";
    file.close();
    return true;
}

Account* AccountDAO::read(const string &id) {
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i]->getUsername() == id || dataCache[i]->getUserID() == id) return dataCache[i];
    }
    return nullptr;
}

bool AccountDAO::update(const string &id, Account* entity) {
    return saveData();
}

bool AccountDAO::remove(const string &id) {
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i]->getUsername() == id) {
            delete dataCache[i];
            for (int j = i; j < dataCache.getSize() - 1; ++j) dataCache[j] = dataCache[j+1];
            dataCache.Pop_back();
            return saveData();
        }
    }
    return true;
}

bool AccountDAO::loadData() {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file " << filename << endl;
        return false;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string username, password, userType, userID;
        getline(ss, username, '|');
        getline(ss, password, '|');
        getline(ss, userType, '|');
        getline(ss, userID);

        Account *acc = new Account();
        acc->setUsername(username);
        acc->setPassword(password);
        acc->setUserType(userType);
        acc->setUserID(userID);
        dataCache.Push_back(acc);
    }
    file.close();
    return true;
}
bool AccountDAO::saveData() {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open " << filename << endl;
        return false;
    }
    for (int i = 0; i < dataCache.getSize(); ++i) {
        file << dataCache[i]->getUsername() << "|" << dataCache[i]->getPassword() << "|" << dataCache[i]->getUserType() << "|" << dataCache[i]->getUserID() << "\n";
    }
    file.close();
    return true;
}
