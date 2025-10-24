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
    Category newCategory;
    cin >> newCategory;

    // Check duplicate
    if (categoryDAO.read(newCategory.getID_Category()) != nullptr) {
        cout << "Category ID already exists.\n";
        return;
    }

    if (categoryDAO.addCategory(newCategory.getID_Category(), newCategory.getName_Category())) {
        cout << "Category added successfully.\n";
    } else {
        cout << "Failed to add category.\n";
    }

}

void ShowCategoryList(CategoryDAO &categoryDAO) {
    categoryDAO.displayAll();
}

// Stream I/O for Category
ostream& Category::output(ostream& os) const {
    os << left << setw(10) << ID_Category << setw(20) << Name_Category;
    return os;
}

ostream& operator<<(ostream& os, const Category& c) {
    return c.output(os);
}

istream& Category::input(istream& is) {
    cout << "Enter Category ID: "; getline(is, ID_Category);
    cout << "Enter Category Name: "; getline(is, Name_Category);
    return is;
}
istream& operator>>(istream& is, Category& c) { return c.input(is); }

