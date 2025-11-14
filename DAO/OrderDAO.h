#ifndef ORDERDAO_H_INCLUDED
#define ORDERDAO_H_INCLUDED
#include "IDAO.h"
#include "CustomerDAO.h"
#include "StaffDAO.h"
#include "ProductDAO.h"
#include "../Models/Order.h"

class OrderDAO : public VectorDAO<Order> {
public:
    OrderDAO(const string& file) : VectorDAO<Order>(file) {}

    bool create(const string &id, Order* entity) override;
    Order* read(const string &id) override;
    bool update(const string &id, Order* entity) override;
    bool remove(const string &id) override;

    // loadAll needs references to customers and products to resolve pointers
    bool loadData(CustomerDAO &customerDAO, StaffDAO &staffDAO, ProductDAO &productDAO);
    bool loadData() override; // Not used but required by IDAO
    bool saveData() override;
    // Remove an order by pointer (useful when orders may have placeholder IDs)
    bool removeByPointer(Order* ord);
};

#endif // ORDERDAO_H_INCLUDED
