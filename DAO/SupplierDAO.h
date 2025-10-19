#ifndef SUPPLIERDAO_H_INCLUDED
#define SUPPLIERDAO_H_INCLUDED
#include "IDAO.h"
#include "../DataStructure/MyMap.h"
#include "../Models/Supplier.h"

class SupplierDAO : public MapDAO {
public:
    SupplierDAO(const string& file) : MapDAO(file) {}

    // Get supplier name by ID
    string getSupplierName(const string& id);

    // Add supplier (wrapper cho create)
    bool addSupplier(const string& id, const string& name);

    // Update supplier name
    bool updateSupplier(const string& id, const string& newName);

    // Delete supplier
    bool deleteSupplier(const string& id);

    // Check if supplier exists
    bool supplierExists(const string& id);

    // Display
    void displayAll();
};

#endif // SUPPLIERDAO_H_INCLUDED
