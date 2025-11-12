#ifndef SUPPLIER_H_INCLUDED
#define SUPPLIER_H_INCLUDED
#include <fstream>
#include "../DataStructure/MyMap.h"

class SupplierDAO;

class Supplier {
    private:
        string ID_Supplier;
        string Name_Supplier;
        string Email;
        string phoneNumber;
    public:
        Supplier(
                 string ID_Supplier = "",
                 string Name_Supplier = "",
                 string Email = "",
                 string phoneNumber = ""
        )
        : ID_Supplier(ID_Supplier), Name_Supplier(Name_Supplier), Email(Email), phoneNumber(phoneNumber) {}
        string getSupID() const;
        string getSupName() const;
        string getEmail() const;
        string getPhoneNumber() const;
        string getFormattedPhone() const;
        void setSupID(string SupID);
        void setSupName(string SupName);
        void setEmail(string email);
        void setPhoneNumber(string phone);
        // Stream I/O
        ostream& output(ostream& os) const;
        istream& input(istream& is);
        friend ostream& operator<<(ostream& os, const Supplier& s);
        friend istream& operator>>(istream& is, Supplier& s);
};

// Declarations; implementations are in Supplier.cpp
void AddSupplier(SupplierDAO &supplierDAO);
void ShowSupplierList(SupplierDAO &supplierDAO);
string formatNumber(unsigned long v);

#endif // SUPPLIER_H_INCLUDED
