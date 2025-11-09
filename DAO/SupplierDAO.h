#ifndef SUPPLIERDAO_H_INCLUDED
#define SUPPLIERDAO_H_INCLUDED
#include "IDAO.h"
#include "../DataStructure/MyMap.h"
#include "../Models/Supplier.h"

class SupplierDAO : public MapDAO<Supplier> {
public:
    SupplierDAO(const string& file) : MapDAO<Supplier>(file) {}
    ~SupplierDAO();

    // CRUD operating on Supplier*
    bool create(const string& id, Supplier* entity) override;
    Supplier* read(const string& id) override;
    bool update(const string& id, Supplier* entity) override;
    bool remove(const string& id) override;
    bool loadData() override;
    bool saveData() override;
    void displayAll();
};

#endif // SUPPLIERDAO_H_INCLUDED
