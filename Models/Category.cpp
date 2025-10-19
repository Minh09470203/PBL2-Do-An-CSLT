#include "Category.h"
#include "../DAO/CategoryDAO.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Category member definitions
string Category::getID_Category() { return ID_Category; }
string Category::getName_Category() { return Name_Category; }
void Category::setID_Category(string cateid) { this->ID_Category = cateid; }
void Category::setName_Category(string catename) { this->Name_Category = catename; }


// Free functions
void AddCategory(CategoryDAO &categoryDAO) {
    string id, name;
    cout << "Enter new Category ID: "; getline(cin, id);
    cout << "Enter Category Name: "; getline(cin, name);

    // Check duplicate
    if (categoryDAO.read(id) != nullptr) {
        cout << "Category ID already exists.\n";
        return;
    }

    if (categoryDAO.addCategory(id, name)) {
        cout << "Category added successfully.\n";
    } else {
        cout << "Failed to add category.\n";
    }

}

void ShowCategoryList(CategoryDAO &categoryDAO) {
    categoryDAO.displayAll();
}

