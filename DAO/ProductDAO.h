#ifndef PRODUCTDAO_H_INCLUDED
#define PRODUCTDAO_H_INCLUDED
#include "IDAO.h"
#include "SupplierDAO.h"
#include "CategoryDAO.h"
#include "../DataStructure/MyVector.h"
#include "../Models/Product.h"

class ProductDAO : public VectorDAO<Product> {
public:
    ProductDAO(const string& file) : VectorDAO<Product>(file) {}

    bool create(const string &id, Product* entity) override;
    Product* read(const string &id) override;
    bool update(const string &id, Product* entity) override;
    bool remove(const string &id) override;
    bool loadData(SupplierDAO &supplierDAO, CategoryDAO &categoryDAO);
    bool loadData() override; // Not used but required by IDAO
    bool saveData() override;
};

#endif // PRODUCTDAO_H_INCLUDED