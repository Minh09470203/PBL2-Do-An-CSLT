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

    virtual bool loadData() = 0;
    virtual bool saveData() = 0;
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

    MyVector<T*>& getDataCache() {
        return dataCache;
    }
};


template <typename T>
class MapDAO : public IDAO<T> {
protected:
    string filename;
    MyMap<string, T*> dataCache;
public:
    MapDAO(const string& file) : filename(file) {}

    MyMap<string, T*>& getDataCache() { 
        return dataCache; 
    }
};


#endif // IDAO_H_INCLUDED