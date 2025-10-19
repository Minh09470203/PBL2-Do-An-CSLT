#ifndef PRODUCT_H_INCLUDED
#define PRODUCT_H_INCLUDED
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../DataStructure/MyVector.h"
#include "Category.h"
#include "Supplier.h"
using namespace std;

// Forward declarations for DAO types used in prototypes
class ProductDAO;
class SupplierDAO;
class CategoryDAO;

class Product {
private:
    string ID_SP;
    string Shoe_Name;
    string Color;
    int Size;
    int Price;
    int SLT;
    Supplier* sup;
    Category* cate;
public:
    Product(
            string ID_SP = "",
            string Shoe_Name = "",
            string Color = "",
            unsigned int Size = 0,
            unsigned int Price = 0,
            unsigned int SLT = 0,
            Supplier* sup = nullptr,
            Category* cate = nullptr
    )
    : ID_SP(ID_SP), Shoe_Name(Shoe_Name), Color(Color), Size(Size), Price(Price), SLT(SLT), sup(sup), cate(cate) {}

    ~Product() {
        delete sup;
        delete cate;
    }
    
    void XuatSP();

    string getIDsp();
    string getNamesp();
    string getColor();
    int getSize();
    int getPrice();
    int getQuantity();
    Supplier *getSupplier();
    Category *getCategory();
    void setSLT(int newQuantity);
    void setPrice(int newPrice);
    void setColor(string newColor);
    void setSupplier(Supplier* newSup);
    void setCategory(Category* newCate);
};

// Declarations for product-related functions (implemented in Product.cpp)
void printProductList(ProductDAO &productDAO);
bool isDuplicateProduct(ProductDAO &productDAO, const string& id = "", const string& name = "");
void ProductInsert(ProductDAO &productDAO, SupplierDAO &supplierDAO, CategoryDAO &categoryDAO);
Product* ProductSearchByID(ProductDAO &productDAO, string ma);
void ProductDelete(ProductDAO &productDAO);
#endif // PRODUCT_H_INCLUDED
