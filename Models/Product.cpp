#include "Product.h"
#include "../DAO/ProductDAO.h"
#include "../DAO/SupplierDAO.h"
#include "../DAO/CategoryDAO.h"
#include "Supplier.h"
#include "Category.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Product member definitions

// Stream operators
ostream& Product::output(ostream& os) const {
    os << left << setw(8)  << ID_SP
       << setw(20) << Shoe_Name
       << setw(6)  << Size
       << setw(12) << Price
       << setw(8) << Color
       << setw(18) << SLT
       << setw(15) << (cate ? cate->getName_Category() : string("N/A"))
       << setw(15) << (sup ? sup->getSupName() : string("N/A"));
    return os;
}

ostream& operator<<(ostream& os, const Product& p) {
    return p.output(os);
}
istream& Product::input(istream& is) {
    cout << "Enter ID: "; getline(is, ID_SP);
    cout << "Enter shoe name: "; getline(is, Shoe_Name);
    cout << "Enter color: "; getline(is, Color);
    cout << "Enter size: "; is >> Size; is.ignore();
    cout << "Enter price: "; is >> Price; is.ignore();
    cout << "Enter stock quantity: "; is >> SLT; is.ignore();
    return is;
}

istream& operator>>(istream& is, Product& p) { return p.input(is); }

string Product::getIDsp() { return ID_SP; }
string Product::getNamesp() { return Shoe_Name; }
string Product::getColor() { return Color; }
int Product::getSize() { return Size; }
int Product::getPrice() { return Price; }
int Product::getQuantity() { return SLT; }
Supplier *Product::getSupplier() { return sup; }
Category *Product::getCategory() { return cate; }
void Product::setSLT(int newQuantity) { this->SLT = newQuantity; }
void Product::setPrice(int newPrice) { this->Price = newPrice; }
void Product::setColor(string newColor) { this->Color = newColor; }
void Product::setSupplier(Supplier* newSup) { this->sup = newSup; }
void Product::setCategory(Category* newCate) { this->cate = newCate; }

// Free functions 
void printProductList(ProductDAO &productDAO) {
    MyVector<Product*>& sp = productDAO.getDataCache();
    printHeader();
    for(int i = 0; i < sp.getSize(); i++) {
        cout << *sp[i] << endl;
    }
    cout << string(95, '-') << endl;
}

void printHeader() {
    cout << left << setw(8)  << "ID"
         << setw(20) << "Shoe Name"
         << setw(6)  << "Size"
         << setw(12) << "Price"
         << setw(8) << "Color"
         << setw(18) << "Stock Quantity"
         << setw(15) << "Category"
         << setw(15) << "Supplier" << endl;
    cout << string(95, '-') << endl;
}

bool isDuplicateProduct(ProductDAO &productDAO, const string& id, const string& name) {
    MyVector<Product*>& sp = productDAO.getDataCache();
    for (int i = 0; i < sp.getSize(); ++i) {
        if ((id.size() && sp[i]->getIDsp() == id) || (name.size() && sp[i]->getNamesp() == name)) {
            return true;
        }
    }
    return false;
}

void ProductInsert(ProductDAO &productDAO, SupplierDAO &supplierDAO, CategoryDAO &categoryDAO) {
    MyVector<Product*>& sp = productDAO.getDataCache();
    int n;
    cout << "Enter number of products to add: ";
    cin >> n;
    cin.ignore();
    for(int i = 0; i < n; i++) {
        string masp, tensp, color, SupID, CateID;
        int gia, soluongton, shoeSize;

        cout << "Enter ID: "; getline(cin, masp);
        if (isDuplicateProduct(productDAO, masp)) {
            cout << "The product ID already exists. Please enter again." << endl;
            --i;
            continue;
        }

        cout << "Enter shoe name: "; getline(cin, tensp);
        if (isDuplicateProduct(productDAO, "", tensp)) {
            cout << "The product name already exists. Please enter again." << endl;
            --i;
            continue;
        }
        cout << "Enter color: "; getline(cin, color);
        do {
            cout << "Enter price: "; cin >> gia;
            if (gia < 0) cout << "The price must be > 0!" << endl;
        }while(gia < 0);
        do {
            cout << "Enter stock quantity: "; cin >> soluongton;
            if (soluongton < 0) cout << "The stock quantity must be > 0!" << endl;
        }while(soluongton < 0);
        do {
            cout << "Enter size: "; cin >> shoeSize;
            if (shoeSize < 0) cout << "The size must be > 0!" << endl;
        }while(shoeSize < 0);
        cin.ignore();
        cout << "Enter CateID: "; getline(cin, CateID);
        cout << "Enter SupID: "; getline(cin, SupID);

    Category *cate = new Category();
    cate->setID_Category(CateID);
    Category* cateFromDao = categoryDAO.read(CateID);
    cate->setName_Category(cateFromDao ? cateFromDao->getName_Category() : "Unknown Category");

        // create a product-local Supplier copy from DAO (so Product destructor can still delete it)
        Supplier *sup = nullptr;
        Supplier *supFromDao = supplierDAO.read(SupID);
        if (supFromDao) {
            sup = new Supplier(supFromDao->getSupID(), supFromDao->getSupName(), supFromDao->getAddress(), supFromDao->getEmail());
        } else {
            sup = new Supplier();
            sup->setSupID(SupID);
            sup->setSupName("Unknown Supplier");
        }

        Product *p = new Product(masp, tensp, color, shoeSize, gia, soluongton, sup, cate);
        if (productDAO.create(masp, p)) {
            cout << "Product added successfully." << endl;
        } else {
            cout << "Failed to add product." << endl;
            delete p;
            --i;
        }
    }
}

Product* ProductSearchByIDName(ProductDAO &productDAO, string key) {
    MyVector<Product*>& sp = productDAO.getDataCache();
    for (int i = 0; i < sp.getSize(); i++) {
        if (sp[i]->getIDsp() == key || sp[i]->getNamesp() == key) {
            return sp[i];
        }
    }
    return nullptr;
}

void SearchProductByPriceRange(ProductDAO &productDAO) {
    MyVector<Product*>& sp = productDAO.getDataCache();
    MyVector<Product*> results;

    unsigned int minPrice, maxPrice;
    cout << "Enter minimum price: ";
    cin >> minPrice;
    cout << "Enter maximum price: ";
    cin >> maxPrice;
    cin.ignore();

    for (int i = 0; i < sp.getSize(); i++) {
        int price = sp[i]->getPrice();
        if (price >= minPrice && price <= maxPrice) {
            results.Push_back(sp[i]);
        }
    }

    if (results.Empty()) {
        cout << "No products found in the given price range." << endl;
    } else {
        cout << "Products found in the given price range:" << endl;
        printHeader();
        for (int i = 0; i < results.getSize(); i++) {
            cout << *results[i] << endl;
        }
    }
}

void ProductDelete(ProductDAO &productDAO) {
    MyVector<Product*>& sp = productDAO.getDataCache();
    if (sp.Empty()) {
        cout << "The product list is empty." << endl;
        return;
    }

    string ma;
    cout << "Enter the product ID to delete: ";
    getline(cin, ma);

    if (productDAO.remove(ma)) {
        cout << "Product deleted successfully." << endl;
    } else {
        cout << "Product ID not found." << endl;
    }
}

