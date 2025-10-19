#ifndef CATEGORYDAO_H_INCLUDED
#define CATEGORYDAO_H_INCLUDED
#include "IDAO.h"
#include "../DataStructure/MyMap.h"
#include "../Models/Category.h"

class CategoryDAO : public MapDAO {
public:
    CategoryDAO(const string& file) : MapDAO(file) {}
    // Get category name by ID
    string getCategoryName(const string& id);
    
    // Add category (wrapper cho create)
    bool addCategory(const string& id, const string& name);
    
    // Update category name
    bool updateCategory(const string& id, const string& newName);
    
    // Delete category
    bool deleteCategory(const string& id);

    // Check if category exists
    bool categoryExists(const string& id);

    // Display
    void displayAll();
};

#endif // CATEGORYDAO_H_INCLUDED
