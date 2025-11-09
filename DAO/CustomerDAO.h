#ifndef CUSTOMERDAO_H_INCLUDED
#define CUSTOMERDAO_H_INCLUDED
#include "IDAO.h"
#include "../DataStructure/MyVector.h"
#include "../Models/Customer.h"

class CustomerDAO : public VectorDAO<Customer> {
public:
    CustomerDAO(const string& file) : VectorDAO<Customer>(file) {}
    bool create(const string &id, Customer* entity) override;
    Customer* read(const string &id) override;
    bool update(const string &id, Customer* entity) override;
    bool remove(const string &id) override;
    bool loadData() override;
    bool saveData() override;
};

#endif // CUSTOMERDAO_H_INCLUDED