#ifndef INVOICEDAO_H_INCLUDED
#define INVOICEDAO_H_INCLUDED
#include "IDAO.h"
#include "../DataStructure/MyVector.h"
#include "../Models/Invoice.h"

class InvoiceDAO : public VectorDAO<Invoice> {
public:
    InvoiceDAO(const string& file) : VectorDAO<Invoice>(file) {}

    bool create(const string &id, Invoice* entity) override;
    Invoice* read(const string &id) override;
    bool update(const string &id, Invoice* entity) override;
    bool remove(const string &id) override;

    // loadAll needs references to customers, staff and products to resolve pointers
    bool loadAll(CustomerDAO &customerDAO, StaffDAO &staffDAO, ProductDAO &productDAO);
    bool loadAll() override; // Not used but required by IDAO
    bool saveAll() override;
};

#endif // INVOICEDAO_H_INCLUDED
