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
    bool loadData(CustomerDAO &customerDAO, StaffDAO &staffDAO, ProductDAO &productDAO);
    bool loadData() override; // Not used but required by IDAO
    bool saveData() override;
    // Remove an invoice by pointer (useful when invoices may have placeholder IDs)
    bool removeByPointer(Invoice* inv);
};

#endif // INVOICEDAO_H_INCLUDED
