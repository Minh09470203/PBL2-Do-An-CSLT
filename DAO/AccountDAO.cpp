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
    file << entity->username << "|" << entity->password << "|" << entity->userType << "|" << entity->userID << "\n";
    file.close();
    return true;
}

Account* AccountDAO::read(const string &id) {
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i]->username == id || dataCache[i]->userID == id) return dataCache[i];
    }
    return nullptr;
}

bool AccountDAO::update(const string &id, Account* entity) {
    return saveAll();
}

bool AccountDAO::remove(const string &id) {
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i]->username == id) {
            delete dataCache[i];
            for (int j = i; j < dataCache.getSize() - 1; ++j) dataCache[j] = dataCache[j+1];
            dataCache.Pop_back();
            return saveAll();
        }
    }
    return true;
}

bool AccountDAO::loadAll() {
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
        acc->username = username;
        acc->password = password;
        acc->userType = userType;
        acc->userID = userID;
        dataCache.Push_back(acc);
    }
    file.close();
    return true;
}

bool AccountDAO::saveAll() {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open " << filename << endl;
        return false;
    }
    for (int i = 0; i < dataCache.getSize(); ++i) {
        file << dataCache[i]->username << "|" << dataCache[i]->password << "|" << dataCache[i]->userType << "|" << dataCache[i]->userID << "\n";
    }
    file.close();
    return true;
}
