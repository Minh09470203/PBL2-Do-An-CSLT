#ifndef SUPPLIER_H_INCLUDED
#define SUPPLIER_H_INCLUDED
#include <fstream>
#include "../DataStructure/MyMap.h"

class SupplierDAO;

class Supplier {
    private:
        string ID_Supplier;
        string Name_Supplier;
        string Address;
        string SDT;
    public:
        Supplier(
                 string ID_Supplier = "",
                 string Name_Supplier = "",
                 string Address = "",
                 string SDT = ""
        )
        : ID_Supplier(ID_Supplier), Name_Supplier(Name_Supplier), Address(Address), SDT(SDT) {}
        string getSupID();
        string getSupName();
        void setSupID(string SupID);
        void setSupName(string SupName);
};

// Declarations; implementations are in Supplier.cpp
void AddSupplier(SupplierDAO &supplierDAO);
void ShowSupplierList(SupplierDAO &supplierDAO);

#endif // SUPPLIER_H_INCLUDED
