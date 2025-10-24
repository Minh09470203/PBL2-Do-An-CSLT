#ifndef ORDERDETAIL_H_INCLUDED
#define ORDERDETAIL_H_INCLUDED
#include <iostream>
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

        unsigned long Subtotal() const;
        unsigned int getQuantity() const;
        unsigned long getUnitPrice() const;
        Product *getProduct() const;
        // Stream I/O
        ostream& output(ostream& os) const;
        friend ostream& operator<<(ostream& os, const OrderDetail& od);
};

#endif // ORDERDETAIL_H_INCLUDED
