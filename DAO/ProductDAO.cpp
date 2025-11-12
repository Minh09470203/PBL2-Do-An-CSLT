#include "ProductDAO.h"

bool ProductDAO::create(const string &id, Product* entity) {
    dataCache.Push_back(entity);
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "Cannot open " << filename << endl;
        return false;
    }
    file << entity->getIDsp() << "|"
         << entity->getNamesp() << "|"
         << entity->getColor() << "|"
         << entity->getSize() << "|"
         << entity->getPrice() << "|"
         << entity->getQuantity() << "|"
         << (entity->getSupplier() ? entity->getSupplier()->getSupID() : string("")) << "|"
         << (entity->getCategory() ? entity->getCategory()->getID_Category() : string("")) << "\n";
    file.close();
    return true;
}

Product* ProductDAO::read(const string &id) {
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i]->getIDsp() == id) {
            // Found the product
            return dataCache[i];
        }
    }
    return nullptr;
}

bool ProductDAO::update(const string &id, Product* entity) {
    // Implementation for updating a product
    return true;
}

bool ProductDAO::remove(const string &id) {
    for (int i = 0; i < dataCache.getSize(); ++i) {
        if (dataCache[i]->getIDsp() == id) {
            // Found the product to remove
            delete dataCache[i];
            for (int j = i; j < dataCache.getSize() - 1; ++j) {
                dataCache[j] = dataCache[j + 1];
            }
            dataCache.Pop_back();
            return saveData();
        }
    }
    return true;
}

bool ProductDAO::loadData(SupplierDAO &supplierDAO, CategoryDAO &categoryDAO) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, color, sizeStr, priceStr, qtyStr, supID, cateID;

        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, color, '|');
        getline(ss, sizeStr, '|');
        getline(ss, priceStr, '|');
        getline(ss, qtyStr, '|');
        getline(ss, supID, '|');
        getline(ss, cateID);

        // Build a product-local Supplier copy from DAO (preserve existing ownership semantics)
        Supplier *sup = nullptr;
        Supplier *supFromDao = supplierDAO.read(supID);
        if (supFromDao) {
            // Supplier constructor: ID, Name, Email, Phone
            sup = new Supplier(supFromDao->getSupID(), supFromDao->getSupName(), supFromDao->getEmail(), supFromDao->getPhoneNumber());
        } else {
            sup = new Supplier();
            sup->setSupID(supID);
            sup->setSupName("Unknown Supplier");
        }

        Category *cate = new Category();
        cate->setID_Category(cateID);
        Category* cateFromDao = categoryDAO.read(cateID);
        cate->setName_Category(cateFromDao ? cateFromDao->getName_Category() : "Unknown Category");

        Product *p = new Product(id, name, color, stoi(sizeStr), stoul(priceStr), stoi(qtyStr), sup, cate);
        dataCache.Push_back(p);
    }
    file.close();
    return true;
}

bool ProductDAO::loadData() {
    // Not used but required by IDAO
    return true;
}

bool ProductDAO::saveData() {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open " << filename << endl;
        return false;
    }

    for (int i = 0; i < dataCache.getSize(); ++i) {
        file << dataCache[i]->getIDsp() << "|"
             << dataCache[i]->getNamesp() << "|"
             << dataCache[i]->getColor() << "|"
             << dataCache[i]->getSize() << "|"
             << dataCache[i]->getPrice() << "|"
             << dataCache[i]->getQuantity() << "|"
             << (dataCache[i]->getSupplier() ? dataCache[i]->getSupplier()->getSupID() : string("")) << "|"
             << (dataCache[i]->getCategory() ? dataCache[i]->getCategory()->getID_Category() : string("")) << "\n";
    }
    file.close();
    return true;
}
