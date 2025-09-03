#ifndef ORDERDETAIL_H_INCLUDED
#define ORDERDETAIL_H_INCLUDED
#include "Product.h"

class OrderDetail {
    private:
        Product *SP;
        unsigned int Quantity;
        unsigned long UnitPrice;
    public:
        OrderDetail(
                    Product *SP = nullptr,
                    unsigned int Quantity = 0,
                    unsigned long UnitPrice = 0
        )
        : SP(SP), Quantity(Quantity), UnitPrice(UnitPrice) {}

        unsigned long Subtotal() const {
            return Quantity * UnitPrice;
        }
        unsigned int getQuantity() const {
            return Quantity;
        }
        unsigned long getUnitPrice() const {
            return UnitPrice;
        }
        Product *getProduct() const {
            return SP;
        }
        void display() const {
            cout << SP->getNamesp() << " | Quantity: " << Quantity
                 << " | Unit price: " << UnitPrice
                 << " | Subtotal: " << Subtotal() << endl;
        }
};

#endif // ORDERDETAIL_H_INCLUDED
