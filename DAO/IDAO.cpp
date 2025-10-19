#include "IDAO.h"

using namespace std;

// MapDAO method definitions
bool MapDAO::create(const string& id, string* name) {
    if (dataCache.Find(id)) {
        cerr << "ID already exists: " << id << endl;
        return false;
    }
    dataCache.Insert(id, *name);
    // Append to file
    ofstream file(filename, ios::app);
    if (!file.is_open()) return false;
    file << id << "|" << *name << "\n";
    file.close();
    return true;
}

string* MapDAO::read(const string& id) {
    return dataCache.Find(id);
}

bool MapDAO::update(const string& id, string* newName) {
    if (!dataCache.Find(id)) {
        cerr << "ID not found: " << id << endl;
        return false;
    }
    dataCache[id] = *newName;
    return saveAll();
}

bool MapDAO::remove(const string& id) {
    if (!dataCache.Find(id)) {
        cerr << "ID not found: " << id << endl;
        return false;
    }
    if (dataCache.Remove(id)) {
        return saveAll();
    }
    return false;
}

bool MapDAO::loadAll() {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        return false;
    }
    dataCache.Clear();
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name;
        getline(ss, id, '|');
        getline(ss, name);
        dataCache.Insert(id, name);
    }
    file.close();
    return true;
}

bool MapDAO::saveAll() {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot write to: " << filename << endl;
        return false;
    }
    dataCache.WriteToFile(file);
    file.close();
    return true;
}
