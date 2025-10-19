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
            return saveAll();
        }
    }
    return true;
}

bool ProductDAO::loadAll(SupplierDAO &supplierDAO, CategoryDAO &categoryDAO) {
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

        Supplier *sup = new Supplier();
        sup->setSupID(supID);
        string* supName = supplierDAO.read(supID);
        sup->setSupName(supName ? *supName : "Unknown Supplier");

        Category *cate = new Category();
        cate->setID_Category(cateID);
        string* cateName = categoryDAO.read(cateID);
        cate->setName_Category(cateName ? *cateName : "Unknown Category");

        Product *p = new Product(id, name, color, stoi(sizeStr), stoul(priceStr), stoi(qtyStr), sup, cate);
        dataCache.Push_back(p);
    }
    file.close();
    return true;
}

bool ProductDAO::loadAll() {
    // Not used but required by IDAO
    return true;
}

bool ProductDAO::saveAll() {
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
