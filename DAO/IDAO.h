#ifndef IDAO_H_INCLUDED
#define IDAO_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "../DataStructure/MyVector.h"
#include "../DataStructure/MyMap.h"

using namespace std;
template <typename T>
class IDAO { // Interface for Data Access Objects
public:
    // CRUD operations
    virtual ~IDAO() {}
    virtual bool create(const string &id, T* entity) = 0;
    virtual T* read(const string &id) = 0;
    virtual bool update(const string &id, T* entity) = 0;
    virtual bool remove(const string &id) = 0;

    virtual bool loadAll() = 0;
    virtual bool saveAll() = 0;
};

template <typename T>
class VectorDAO : public IDAO<T> {
protected:
    string filename;
    MyVector<T*> dataCache;
public:
    VectorDAO(const string& file) : filename(file) {}

    virtual ~VectorDAO() {
        for (int i = 0; i < dataCache.getSize(); i++) {
            delete dataCache[i];
        }
    }
    // Provide access to the loaded data so callers (e.g. main) can use it
    MyVector<T*>& getDataCache() {
        return dataCache;
    }
};

class MapDAO : public IDAO<string> {
protected:
    string filename;
    MyMap<string, string> dataCache;
public:
    MapDAO(const string& file) : filename(file) {}
    bool create(const string& id, string* name) override;
    string* read(const string& id) override;
    bool update(const string& id, string* newName) override;
    bool remove(const string& id) override;
    bool loadAll() override;
    bool saveAll() override;
    MyMap<string, string>& getdataCache() {
        return dataCache;
    }
};
#endif // IDAO_H_INCLUDED