#ifndef CATEGORY_H_INCLUDED
#define CATEGORY_H_INCLUDED
#include <fstream>
#include <sstream>
#include "../DataStructure/MyMap.h"

class CategoryDAO;

class Category {
    private:
        string ID_Category;
        string Name_Category;
    public:
        Category(
                 string ID_Category = "",
                 string Name_Category = ""
        )
        : ID_Category(ID_Category), Name_Category(Name_Category) {}
        string getID_Category();
        string getName_Category();
        void setID_Category(string cateid);
        void setName_Category(string catename);
        // Stream I/O
        ostream& output(ostream& os) const;
        istream& input(istream& is);
        friend ostream& operator<<(ostream& os, const Category& c);
        friend istream& operator>>(istream& is, Category& c);

};

// Declarations - implementations in Category.cpp
void AddCategory(CategoryDAO &categoryDAO);
void ShowCategoryList(CategoryDAO &categoryDAO);

#endif // CATEGORY_H_INCLUDED
