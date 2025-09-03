#ifndef PRODUCT_H_INCLUDED
#define PRODUCT_H_INCLUDED
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Category.h"
#include "Supplier.h"
using namespace std;

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

    void XuatSP() {
        cout << left << setw(8)  << ID_SP
             << setw(20) << Shoe_Name
             << setw(6)  << Size
             << setw(12) << Price
             << setw(8)  << Color
             << setw(18) << SLT
             << setw(15) << (cate ? cate->getName_Category() : "N/A")
             << setw(15) << (sup ? sup->getSupName() : "N/A") << endl;
    }

    string getIDsp() { return ID_SP; }
    string getNamesp() { return Shoe_Name; }
    string getColor() { return Color; }
    int getSize() { return Size; }
    int getPrice() { return Price; }
    int getQuantity() { return SLT; }
    Supplier *getSupplier() { return sup; }
    Category *getCategory() { return cate; }
    void setSLT(int newQuantity) { this->SLT = newQuantity; }
    void setPrice(int newPrice) { this->Price = newPrice; }
    void setColor(string newColor) { this->Color = newColor; }
    void setSupplier(Supplier* newSup) { this->sup = newSup; }
    void setCategory(Category* newCate) { this->cate = newCate; }
};

void printProductList(MyVector<Product*> &sp) {
    cout << left << setw(8)  << "ID"
         << setw(20) << "Shoe Name"
         << setw(6)  << "Size"
         << setw(12) << "Price"
         << setw(8) << "Color"
         << setw(18) << "Stock Quantity"
         << setw(15) << "Category"
         << setw(15) << "Supplier" << endl;
    cout << string(95, '-') << endl;
    for(int i = 0; i < sp.getSize(); i++) {
        sp[i]->XuatSP();
    }
    cout << string(95, '-') << endl;
}

bool isDuplicateProduct(MyVector<Product*>& sp, const string& id) {
    for (int i = 0; i < sp.getSize(); ++i) {
        if (sp[i]->getIDsp() == id) {
            return true;
        }
    }
    return false;
}

// FIXED: ProductInsert function
void ProductInsert(MyVector<Product*> &sp, int n) {
    ofstream file("Data/Product.txt", ios::app);
    if (!file.is_open()) {
        cout << "Cannot open Product.txt" << endl;
        return;
    }

    for(int i = 0; i < n; i++) {
        string masp, tensp, color, SupID, CateID;
        int gia, soluongton, shoeSize;

        cout << "Enter ID: "; getline(cin, masp);
        if (isDuplicateProduct(sp, masp)) {
            cout << "The product ID already exists. Please enter again." << endl;
            --i;
            continue;
        }

        cout << "Enter shoe name: "; getline(cin, tensp);
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
        cate->setName_Category(categoryMap[CateID]);

        Supplier *sup = new Supplier();
        sup->setSupID(SupID);
        sup->setSupName(supplierMap[SupID]);

        Product *p = new Product(masp, tensp, color, shoeSize, gia, soluongton, sup, cate);
        sp.Push_back(p);

        file << masp << "|" << tensp << "|" << color << "|" << shoeSize << "|"
             << gia << "|" << soluongton << "|" << SupID << "|" << CateID << "\n";
    }
    file.close();
    cout << "Product added successfully." << endl;
}

Product* ProductSearchByID(MyVector<Product*> &sp, string ma) {
    for(int i = 0; i < sp.getSize(); i++) {
        if (ma == sp[i]->getIDsp()) {
            return sp[i];
        }
    }
    return nullptr;
}

bool loadProductFile(const string& filename, MyVector<Product*> &sp) {
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
        sup->setSupName(supplierMap[supID]);

        Category *cate = new Category();
        cate->setID_Category(cateID);
        cate->setName_Category(categoryMap[cateID]);

        Product *p = new Product(id, name, color, stoi(sizeStr),
                                stoul(priceStr), stoi(qtyStr), sup, cate);
        sp.Push_back(p);
    }
    file.close();
    return true;
}

// FIXED: ProductDelete function
void ProductDelete(MyVector<Product*> &sp) {
    if (sp.Empty()) {
        cout << "The product list is empty." << endl;
        return;
    }

    string ma;
    cout << "Enter the product ID to delete: ";
    getline(cin, ma);

    bool found = false;
    for (int i = 0; i < sp.getSize(); i++) {
        if (sp[i]->getIDsp() == ma) {
            found = true;
            delete sp[i]->getSupplier();
            delete sp[i]->getCategory();
            delete sp[i];

            // Shift elements forward
            for (int j = i; j < sp.getSize() - 1; j++) {
                sp[j] = sp[j + 1];
            }

            cout << "Product with ID " << ma << " has been deleted successfully." << endl;
            break;
        }
    }

    if (!found) {
        cout << "Product with ID " << ma << " not found." << endl;
        return;
    }

    // Rewrite entire Product.txt file
    ofstream file("Data/Product.txt");
    if (!file.is_open()) {
        cout << "Cannot open Product.txt" << endl;
        return;
    }

    for (int i = 0; i < sp.getSize() - 1; i++) { // -1 because we removed one item
        Product* p = sp[i];
        file << p->getIDsp() << "|" << p->getNamesp() << "|" << p->getColor() << "|"
             << p->getSize() << "|" << p->getPrice() << "|" << p->getQuantity() << "|"
             << p->getSupplier()->getSupID() << "|" << p->getCategory()->getID_Category() << "\n";
    }
    file.close();
}
#endif // PRODUCT_H_INCLUDED
