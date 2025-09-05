#ifndef INVOICE_H_INCLUDED
#define INVOICE_H_INCLUDED
#include "Customer.h"
#include "Staff.h"
#include "Product.h"
#include "OrderDetail.h"

class Invoice {
private:
    string ID_HD;
    string Date;
    Customer *kh;
    Staff *nv;
    unsigned long TotalAmount;
    MyVector<OrderDetail*> details;
public:
    Invoice(
            string ID_HD = "",
            string Date = "",
            Customer *kh = nullptr,
            Staff *nv = nullptr,
            unsigned long TotalAmount = 0
    )
    : ID_HD(ID_HD), Date(Date), kh(kh), nv(nv), TotalAmount(TotalAmount) {}

    string getIDhd() { return ID_HD; }
    string getDate() { return Date; }
    Customer *getCustomer() { return kh; }
    Staff *getStaff() { return nv; }
    MyVector<OrderDetail*>& getDetails() { return details; }
    unsigned long getTotalAmount() { return TotalAmount; }

    void setInfo(string id, string date, Customer *customer, Staff *staff) {
        this->ID_HD = id;
        this->Date = date;
        this->kh = customer;
        this->nv = staff;
    }

    void calculateTotal() {
        TotalAmount = 0;
        for(int i = 0; i < details.getSize(); i++) {
            TotalAmount += details[i]->Subtotal();
        }
    }

    void PrintHD() {
        cout << "\nInvoice: " << ID_HD << "\nDate: " << Date
             << "\nCustomer: " << kh->getName() << "\nStaff: " << nv->getNamenv()
             << "\n--- Order detail ---\n";
        for(int i = 0; i < details.getSize(); i++) {
            details[i]->display();
        }
        cout << "Total amount: " << TotalAmount << " VND" << endl;
    }

    void addDetail(OrderDetail* od) {
        details.Push_back(od);
    }
};

void CreateInvoice(MyVector<Invoice*>& invoices,
                   MyVector<Customer*>& dsKH,
                   MyVector<Staff*>& dsNV,
                   MyVector<Product*>& dsSP)
{
    string invID, date, custID, staffID;
    cout << "Enter Invoice ID: ";
    getline(cin, invID);
    cout << "Enter Date (dd/mm/yyyy): ";
    getline(cin, date);

    // Find customer
    cout << "Enter Customer ID: ";
    getline(cin, custID);
    Customer* cust = nullptr;
    for (int i = 0; i < dsKH.getSize(); ++i) {
        if (dsKH[i]->getID() == custID) {
            cust = dsKH[i];
            break;
        }
    }
    if (!cust) {
        cout << "Customer ID not found. Aborting invoice creation.\n";
        return;
    }

    // Find staff
    cout << "Enter Staff ID: ";
    getline(cin, staffID);
    Staff* staff = nullptr;
    for (int i = 0; i < dsNV.getSize(); ++i) {
        if (dsNV[i]->getIDnv() == staffID) {
            staff = dsNV[i];
            break;
        }
    }
    if (!staff) {
        cout << "Staff ID not found. Aborting invoice creation.\n";
        return;
    }

    // Create Invoice object
    Invoice* inv = new Invoice();
    inv->setInfo(invID, date, cust, staff);

    int itemCount;
    cout << "Enter number of products in invoice: ";
    cin >> itemCount;
    cin.ignore();

    for (int i = 0; i < itemCount; ++i) {
        string pid;
        int qty;
        cout << "\nProduct #" << i + 1 << "\n";
        cout << "  Enter Product ID: ";
        getline(cin, pid);

        // Find product
        Product* prod = nullptr;
        for (int j = 0; j < dsSP.getSize(); ++j) {
            if (dsSP[j]->getIDsp() == pid) {
                prod = dsSP[j];
                break;
            }
        }
        if (!prod) {
            cout << "  Product ID not found. Please re-enter this item.\n";
            --i;
            continue;
        }

        cout << "  Enter Quantity: ";
        cin >> qty;
        cin.ignore();
        if (qty == 0 || qty > prod->getQuantity()) {
            cout << "  Invalid quantity (available: "
                 << prod->getQuantity() << "). Please re-enter this item.\n";
            --i;
            continue;
        }

        // Add orderdetail
        OrderDetail* od = new OrderDetail(prod, qty, prod->getPrice());
        inv->addDetail(od);

        // Update inventory quantity
        prod->setSLT(prod->getQuantity() - qty);
    }

    // Calculate total amount
    inv->calculateTotal();
    invoices.Push_back(inv);

    // Write the invoice to a file
    ofstream fout("Data/Invoice.txt", ios::app);
    if (!fout.is_open()) {
        cout << "Cannot open Invoice.txt for writing.\n";
        return;
    }

    // Order line: ID|Date|CustomerID|StaffID|TotalAmount
    fout << inv->getIDhd() << "|"
         << inv->getDate() << "|"
         << inv->getCustomer()->getID() << "|"
         << inv->getStaff()->getIDnv() << "|"
         << inv->getTotalAmount() << "\n";

    // Order detail lines: -ProductID|Quantity|UnitPrice
    for (int k = 0; k < inv->getDetails().getSize(); k++) {
        OrderDetail* od = inv->getDetails()[k];
        fout << "-" << od->getProduct()->getIDsp() << "|"
             << od->getQuantity() << "|"
             << od->getUnitPrice() << "\n";
    }

    fout.close();
    // Save updated product quantities to file
    ofstream prodFile("Data/Product.txt");
    if (prodFile.is_open()) {
        for (int i = 0; i < dsSP.getSize(); i++) {
            Product* p = dsSP[i];
            prodFile << p->getIDsp() << "|" << p->getNamesp() << "|"
                     << p->getColor() << "|" << p->getSize() << "|"
                     << p->getPrice() << "|" << p->getQuantity() << "|"
                     << p->getSupplier()->getSupID() << "|"
                     << p->getCategory()->getID_Category() << "\n";
        }
        prodFile.close();
    }
    cout << "\nInvoice " << inv->getIDhd()
         << " created and saved successfully.\n";
}

void printInvoiceList(MyVector<Invoice*> &invoices) {
    cout << left << setw(15) << "Invoice ID"
         << setw(12) << "Date"
         << setw(18) << "Customer"
         << setw(18) << "Staff"
         << setw(12) << "Total Amount" << endl;
    cout << string(82, '-') << endl;
    for (int i = 0; i < invoices.getSize(); i++) {
        cout << left << setw(15) << invoices[i]->getIDhd()
             << setw(12) << invoices[i]->getDate()
             << setw(18) << invoices[i]->getCustomer()->getName()
             << setw(18) << invoices[i]->getStaff()->getNamenv()
             << setw(12) << invoices[i]->getTotalAmount() << "VND" << endl;
        cout << string(82, '-') << endl;
    }
}

bool loadInvoiceFile(const string& filename, MyVector<Invoice*> &invoices,
                     MyVector<Customer*> &dsKH, MyVector<Staff*> &dsNV,
                     MyVector<Product*> &dsSP) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file " << filename << endl;
        return false;
    }

    string line;
    Invoice* currentInvoice = nullptr;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line[0] != '-') {
            // This is an invoice header line
            stringstream ss(line);
            string invID, date, custID, staffID, totalStr;
            getline(ss, invID, '|');
            getline(ss, date, '|');
            getline(ss, custID, '|');
            getline(ss, staffID, '|');
            getline(ss, totalStr);

            // Find customer and staff
            Customer* cust = nullptr;
            Staff* staff = nullptr;

            for (int i = 0; i < dsKH.getSize(); i++) {
                if (dsKH[i]->getID() == custID) {
                    cust = dsKH[i];
                    break;
                }
            }

            for (int i = 0; i < dsNV.getSize(); i++) {
                if (dsNV[i]->getIDnv() == staffID) {
                    staff = dsNV[i];
                    break;
                }
            }

            if (cust && staff) {
                currentInvoice = new Invoice(invID, date, cust, staff, stoul(totalStr));
                invoices.Push_back(currentInvoice);
            }
        } else {
            // This is an order detail line (starts with '-')
            if (currentInvoice != nullptr) {
                stringstream dss(line.substr(1)); // Remove the '-' character
                string prodID, qtyStr, priceStr;
                getline(dss, prodID, '|');
                getline(dss, qtyStr, '|');
                getline(dss, priceStr);

                // Find product
                Product* prod = nullptr;
                for (int i = 0; i < dsSP.getSize(); i++) {
                    if (dsSP[i]->getIDsp() == prodID) {
                        prod = dsSP[i];
                        break;
                    }
                }

                if (prod) {
                    OrderDetail* od = new OrderDetail(prod, stoi(qtyStr), stoul(priceStr));
                    currentInvoice->addDetail(od);
                }
            }
        }
    }
    file.close();
    return true;
}

#endif // INVOICE_H_INCLUDED
