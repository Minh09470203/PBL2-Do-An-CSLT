#ifndef CATEGORYDAO_H_INCLUDED
#define CATEGORYDAO_H_INCLUDED
#include "IDAO.h"
#include "../DataStructure/MyMap.h"
#include "../Models/Category.h"

class CategoryDAO : public MapDAO<Category> {
public:
    CategoryDAO(const string& file) : MapDAO<Category>(file) {}
    ~CategoryDAO();

    bool create(const string& id, Category* entity) override;
    Category* read(const string& id) override;
    bool update(const string& id, Category* entity) override;
    bool remove(const string& id) override;
    bool loadData() override;
    bool saveData() override;
    void displayAll();
};

#endif // CATEGORYDAO_H_INCLUDED
