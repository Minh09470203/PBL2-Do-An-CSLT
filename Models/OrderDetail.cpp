#include "OrderDetail.h"

using namespace std;    

unsigned long OrderDetail::Subtotal() const {
    return Quantity * UnitPrice;
}

unsigned int OrderDetail::getQuantity() const {
    return Quantity;
}

unsigned long OrderDetail::getUnitPrice() const {
    return UnitPrice;
}

Product *OrderDetail::getProduct() const {
    return SP;
}

ostream& OrderDetail::output(ostream& os) const {
    os << SP->getNamesp() << " | Quantity: " << Quantity
       << " | Unit price: " << UnitPrice
       << " | Subtotal: " << Subtotal();
    return os;
}

ostream& operator<<(ostream& os, const OrderDetail& od) {
    return od.output(os);
}