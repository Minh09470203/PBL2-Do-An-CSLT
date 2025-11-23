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
    string Size;
    int Price;
    int SLT;
    Supplier* sup;
    Category* cate;
public:
    Product(
            string ID_SP = "",
            string Shoe_Name = "",
            string Color = "",
            string Size = "",
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

    string getIDsp();
    string getNamesp();
    string getColor();
    string getSize();
    int getPrice();
    int getSLT();
    Supplier *getSupplier();
    Category *getCategory();
    void setSLT(int newQuantity);
    void setPrice(int newPrice);
    void setColor(string newColor);
    void setSupplier(Supplier* newSup);
    void setCategory(Category* newCate);
    // Stream I/O helpers
    ostream& output(ostream& os) const;
    istream& input(istream& is);
    friend ostream& operator<<(ostream& os, const Product& p);
    friend istream& operator>>(istream& is, Product& p);
};

// Declarations for product-related functions 
void printProductList(ProductDAO &productDAO);
void printHeader();
bool isDuplicateProduct(ProductDAO &productDAO, const string& id = "", const string& name = "");
void ProductInsert(ProductDAO &productDAO, SupplierDAO &supplierDAO, CategoryDAO &categoryDAO);
Product* ProductSearchByKey(ProductDAO &productDAO, string key);
bool ProductSearchPrintedResults();
void SearchProductByPriceRange(ProductDAO &productDAO);
void SearchProductBySupplier(ProductDAO &productDAO);
void SearchProductByCategory(ProductDAO &productDAO);
void ProductDelete(ProductDAO &productDAO);
#endif // PRODUCT_H_INCLUDED
