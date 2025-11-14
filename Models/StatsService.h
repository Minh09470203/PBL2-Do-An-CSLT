#ifndef STATSSERVICE_H_INCLUDED
#define STATSSERVICE_H_INCLUDED

#include <iostream>
#include <string>
#include "../DataStructure/MyVector.h"
#include "../DataStructure/MyMap.h"

// Forward DAO declarations
class SessionManager;
class OrderDAO;
class ProductDAO;
class CustomerDAO;
class StaffDAO;
class CategoryDAO;
class SupplierDAO;

using namespace std;

class StatsService {
private:
    SessionManager* session;
    OrderDAO* orderDao;
    ProductDAO* productDao;
    CustomerDAO* customerDao;
    StaffDAO* staffDao;
    CategoryDAO* categoryDao;
    SupplierDAO* supplierDao;

    // simple date parser dd/mm/yyyy -> d,m,y
    void parseDate(const string &s, int &d, int &m, int &y);

public:
    StatsService(SessionManager &sess);

    unsigned long totalRevenue();
    int orderCount();
    void revenueByMonth(int year, unsigned long months[13]); // months[1..12]

    // Reports printed directly (keep interface simple)
    void printDashboard();
    void printRevenueByMonth(int year);
    void printTopProductsByQty(int N);
    void printTopCustomersByRevenue(int N);
    void printLowStock(int threshold);
    // exportTopProductsCSV removed per user request
};

#endif // STATSSERVICE_H_INCLUDED
