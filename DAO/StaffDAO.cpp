#include "StaffDAO.h"

bool StaffDAO::create(const string &id, Staff* entity) {
    dataCache.Push_back(entity);
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "Cannot open " << filename << endl;
        return false;
    }
    file << entity->getIDnv() << "|"
         << entity->getNamenv() << "|"
         << entity->getPosition() << "|"
         << entity->getPhone() << "\n";
    file.close();
    return true;
}

Staff* StaffDAO::read(const string &id) {
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i]->getIDnv() == id) {
            // Found the staff
            return dataCache[i];
        }
    }
    return nullptr;
}

bool StaffDAO::update(const string &id, Staff* entity) {
    // Implementation for updating a staff
    return true;
}

bool StaffDAO::remove(const string &id) {
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i]->getIDnv() == id) {
            // Found the staff to remove
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

bool StaffDAO::loadData() {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, position, phone;
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, position, '|');
        getline(ss, phone);
        Staff *s = new Staff(id, name, position, phone);
        dataCache.Push_back(s);
    }
    file.close();
    return true;
}
bool StaffDAO::saveData() {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open " << filename << endl;
        return false;
    }

    for (int i = 0; i < dataCache.getSize(); ++i) {
        file << dataCache[i]->getIDnv() << "|"
             << dataCache[i]->getNamenv() << "|"
             << dataCache[i]->getPosition() << "|"
             << dataCache[i]->getPhone() << "\n";
    }
    file.close();
    return true;
}