#include "StatsService.h"
#include "../AppSession/SessionManager.h"
#include "../DAO/InvoiceDAO.h"
#include "../DAO/ProductDAO.h"
#include "../DAO/CustomerDAO.h"
#include "../DAO/StaffDAO.h"
#include "../DAO/CategoryDAO.h"
#include "../DAO/SupplierDAO.h"
#include "Invoice.h"
#include "OrderDetail.h"
#include "Product.h"
#include <iomanip>
#include <cstdlib>

// Use simple, easy-to-read code and only MyMap/MyVector from project

StatsService::StatsService(SessionManager &sess) {
    session = &sess;
    invoiceDao = session->getInvoiceDAO();
    productDao = session->getProductDAO();
    customerDao = session->getCustomerDAO();
    staffDao = session->getStaffDAO();
    categoryDao = session->getCategoryDAO();
    supplierDao = session->getSupplierDAO();
}

void StatsService::parseDate(const string &s, int &d, int &m, int &y) {
    d = m = y = 0;
    int i = 0; string part;
    int parts[3] = {0,0,0};
    int pi = 0;
    while (i < (int)s.size() && pi < 3) {
        char c = s[i];
        if (c == '/' || c == '-' ) {
            if (!part.empty()) parts[pi++] = atoi(part.c_str());
            part.clear();
        } else {
            part.push_back(c);
        }
        ++i;
    }
    if (!part.empty() && pi < 3) parts[pi++] = atoi(part.c_str());
    if (pi == 3) {
        d = parts[0]; m = parts[1]; y = parts[2];
    }
}

unsigned long StatsService::totalRevenue() {
    unsigned long total = 0;
    MyVector<Invoice*>& invs = invoiceDao->getDataCache();
    for (int i = 0; i < invs.getSize(); ++i) {
        total += invs[i]->getTotalAmount();
    }
    return total;
}

int StatsService::invoiceCount() {
    return invoiceDao->getDataCache().getSize();
}

void StatsService::revenueByMonth(int year, unsigned long months[13]) {
    for (int i = 0; i < 13; ++i) months[i] = 0;
    MyVector<Invoice*>& invs = invoiceDao->getDataCache();
    for (int i = 0; i < invs.getSize(); ++i) {
        int d,m,y; parseDate(invs[i]->getDate(), d, m, y);
        if (y == year && m >=1 && m <= 12) months[m] += invs[i]->getTotalAmount();
    }
}

// print helpers
void StatsService::printDashboard() {
    cout << "\n=== DASHBOARD ===\n";
    cout << "Total customers: " << customerDao->getDataCache().getSize() << "\n";
    cout << "Total staff: " << staffDao->getDataCache().getSize() << "\n";
    cout << "Total products: " << productDao->getDataCache().getSize() << "\n";
    cout << "Total invoices: " << invoiceDao->getDataCache().getSize() << "\n";
    cout << "Total revenue: " << totalRevenue() << " VND\n";
}

void StatsService::printRevenueByMonth(int year) {
    unsigned long months[13];
    revenueByMonth(year, months);
    cout << "\n+----------------------------------------+\n";
    cout << "|         REVENUE FOR YEAR " << year << "          |";
    cout << "\n+----------------------------------------+\n";
    // find maximum to scale bars
    unsigned long maxRev = 0;
    for (int m = 1; m <= 12; ++m) if (months[m] > maxRev) maxRev = months[m];
    const int maxBar = 30; // maximum bar width in characters (keep short to avoid wrap)
    for (int m = 1; m <= 12; ++m) {
        unsigned long rev = months[m];
        int barLen = 0;
        if (maxRev > 0) barLen = (int)((__int128)rev * maxBar / maxRev); // avoid overflow
        if (rev > 0 && barLen == 0) barLen = 1; // ensure visible mark for small non-zero values
        if (barLen > maxBar) barLen = maxBar;
        // build bar string using literal block character U+2588
        string bar;
        for (int i = 0; i < barLen; ++i) bar += "#";
        // print line: Month X  [bar]  <amount> VND
        cout << "Month " << setw(2) << m << " ";
        if (!bar.empty()) cout << bar << " ";
        cout << rev << " VND\n";
    }

    // --- Analysis section ---
    unsigned long totalYear = 0;
    for (int m = 1; m <= 12; ++m) totalYear += months[m];

    // previous year comparison
    unsigned long prevMonths[13];
    revenueByMonth(year - 1, prevMonths);
    unsigned long totalPrev = 0;
    for (int m = 1; m <= 12; ++m) totalPrev += prevMonths[m];

    // find max and min months
    int maxMonth = -1, minMonth = -1;
    unsigned long maxVal = 0;
    unsigned long minVal = (unsigned long)(-1);
    for (int m = 1; m <= 12; ++m) {
        unsigned long v = months[m];
        if (maxMonth == -1 || v > maxVal) { maxVal = v; maxMonth = m; }
        if (minMonth == -1 || v < minVal) { minVal = v; minMonth = m; }
    }

    // Print a small ASCII box with analysis (narrower so it doesn't have too much empty space)
    const int aWidth = 40;
    cout << "+" << string(aWidth, '-') << "+\n";
    // Simple fixed padding title
    cout << "|" << string(16, ' ') << "ANALYSIS" << string(16, ' ') << "|\n";
    cout << "+" << string(aWidth, '-') << "+\n";
    // Total revenue
    string totalLine = string("Total revenue: ") + to_string(totalYear) + " VND";
    cout << "| " << left << setw(aWidth-4) << totalLine << "   |\n";

    // Growth vs previous year
    if (totalPrev == 0) {
        string growthLine = string("Growth Vs ") + to_string(year-1) + string(": N/A (no data)");
        cout << "| " << left << setw(aWidth-4) << growthLine << "   |\n";
    } else {
        long long diff = (long long)totalYear - (long long)totalPrev;
        double pct = ((double)diff) * 100.0 / (double)totalPrev;
        char pctbuf[64];
        sprintf(pctbuf, "Growth Vs %d: %.2f%%", year-1, pct);
        cout << "| " << left << setw(aWidth-4) << string(pctbuf) << "   |\n";
    }

    // Highest and lowest months
    if (maxMonth == -1) {
        cout << "| " << left << setw(aWidth-4) << string("No revenue data for this year.") << " |\n";
    } else {
        char buf2[128];
        sprintf(buf2, "Highest month: %02d (%lu VND)", maxMonth, maxVal);
        cout << "| " << left << setw(aWidth-4) << string(buf2) << "   |\n";
        sprintf(buf2, "Lowest month:  %02d (%lu VND)", minMonth, minVal);
        cout << "| " << left << setw(aWidth-4) << string(buf2) << "   |\n";
    }

    cout << "+" << string(aWidth, '-') << "+\n";
}

// For top-N we will aggregate using MyMap and then selection sort on MyVector
struct ProdAgg { string id; string name; unsigned long qty; unsigned long revenue; };

// Helper storage for MyMap traversal callbacks
static MyVector<ProdAgg>* g_prodAggVec = nullptr;
static MyMap<string, unsigned long>* g_revMap = nullptr;

static void collectProdCallback(const string &k, const unsigned long &v) {
    if (!g_prodAggVec) return;
    ProdAgg p; p.id = k; p.qty = v;
    // lookup revenue
    unsigned long rev = 0;
    if (g_revMap) {
        unsigned long* pr = g_revMap->Find(k);
        if (pr) rev = *pr;
    }
    p.revenue = rev;
    p.name = k; // default name = id; we'll try to get actual name below
    g_prodAggVec->Push_back(p);
}

void StatsService::printTopProductsByQty(int N) {
    // Build qty map and revenue map
    MyMap<string, unsigned long> qtyMap; // pid -> qty
    MyMap<string, unsigned long> revMap; // pid -> revenue

    MyVector<Invoice*>& invs = invoiceDao->getDataCache();
    for (int i = 0; i < invs.getSize(); ++i) {
        Invoice* inv = invs[i];
        for (int j = 0; j < inv->getDetails().getSize(); ++j) {
            OrderDetail* od = inv->getDetails()[j];
            if (!od || !od->getProduct()) continue;
            string pid = od->getProduct()->getIDsp();
            qtyMap[pid] = qtyMap[pid] + od->getQuantity();
            revMap[pid] = revMap[pid] + (unsigned long)od->getQuantity() * od->getUnitPrice();
        }
    }

    // collect into vector
    MyVector<ProdAgg> vec;
    g_prodAggVec = &vec; g_revMap = &revMap;
    qtyMap.ForEach(collectProdCallback);
    g_prodAggVec = nullptr; g_revMap = nullptr;

    // Fill product names
    for (int i = 0; i < vec.getSize(); ++i) {
        Product* p = productDao->read(vec[i].id);
        if (p) vec[i].name = p->getNamesp();
    }

    // Selection sort top-N (simple, no STL)
    int total = vec.getSize();
    int limit = (N < total) ? N : total;
    cout << "\nTop " << limit << " products by quantity:\n";
    cout << left << setw(10) << "ID" << setw(30) << "Name" << setw(10) << "Qty" << setw(15) << "Revenue" << "\n";
    cout << string(70, '-') << "\n";
    for (int k = 0; k < limit; ++k) {
        int best = k;
        for (int j = k+1; j < total; ++j) {
            if (vec[j].qty > vec[best].qty) best = j;
        }
        if (best != k) {
            ProdAgg tmp = vec[k]; vec[k] = vec[best]; vec[best] = tmp;
        }
        cout << left << setw(10) << vec[k].id << setw(30) << vec[k].name << setw(10) << vec[k].qty << setw(15) << vec[k].revenue << "\n";
    }
}

// Implement top customers by revenue
void StatsService::printTopCustomersByRevenue(int N) {
    MyMap<string, unsigned long> custMap; // cid -> amount
    MyVector<Invoice*>& invs = invoiceDao->getDataCache();
    for (int i = 0; i < invs.getSize(); ++i) {
        Invoice* inv = invs[i];
        if (!inv->getCustomer()) continue;
        string cid = inv->getCustomer()->getID();
        custMap[cid] = custMap[cid] + inv->getTotalAmount();
    }
    // collect into vector
    struct CustAgg { string id; string name; unsigned long amount; };
    MyVector<CustAgg> vec;
    // callback helper
    static MyVector<CustAgg>* g_custVec = nullptr;
    static MyMap<string, unsigned long>* g_custMapPtr = nullptr;
    g_custMapPtr = &custMap;
    // build vector via ForEach
    // We cannot easily pass closure, so emulate similar to product approach
    // Implement a simple traversal by using ForEach with a static collector
    // Create a local function
    struct LocalHelper { static void collect(const string &k, const unsigned long &v) {
            if (!g_custVec) return; CustAgg c; c.id = k; c.amount = v; c.name = k; g_custVec->Push_back(c);
        }
    };
    g_custVec = &vec;
    custMap.ForEach(LocalHelper::collect);
    g_custVec = nullptr; g_custMapPtr = nullptr;

    for (int i = 0; i < vec.getSize(); ++i) {
        Customer* c = customerDao->read(vec[i].id);
        if (c) vec[i].name = c->getName();
    }

    int total = vec.getSize();
    int limit = (N < total) ? N : total;
    cout << "\nTop " << limit << " customers by revenue:\n";
    cout << left << setw(10) << "ID" << setw(30) << "Name" << setw(15) << "Amount" << "\n";
    cout << string(60, '-') << "\n";
    for (int k = 0; k < limit; ++k) {
        int best = k;
        for (int j = k+1; j < total; ++j) {
            if (vec[j].amount > vec[best].amount) best = j;
        }
        if (best != k) {
            CustAgg tmp = vec[k]; vec[k] = vec[best]; vec[best] = tmp;
        }
        cout << left << setw(10) << vec[k].id << setw(30) << vec[k].name << setw(15) << vec[k].amount << "\n";
    }
}

void StatsService::printLowStock(int threshold) {
    MyVector<Product*>& prods = productDao->getDataCache();
    cout << "\nProducts with stock <= " << threshold << ":\n";
    const int totalWidth = 59; 
    // top border
    cout << "+" << string(totalWidth, '-') << "+\n";
    // header row
    cout << "|" << left << setw(10) << "ID" << "|" << left << setw(30) << "Name" << "|" << left << setw(8) << "Qty" << "|" << left << setw(8) << "Status" << "|\n";
    // separator
    cout << "+" << string(totalWidth, '-') << "+\n";

    // ANSI color codes (may require terminal with ANSI support)
    const string RESET  = "\033[0m";
    const string RED    = "\033[31m";
    const string YELLOW = "\033[33m";
    const string GREEN  = "\033[32m";

    for (int i = 0; i < prods.getSize(); ++i) {
        Product* p = prods[i];
        int qty = p->getQuantity();
        if (qty <= threshold) {
            string status;
            string color;
            if (qty <= 0) {
                status = "Sold out";
                color = RED;
            } else if (qty < 10) {
                status = "Very low";
                color = RED;
            } else if (qty < 15) {
                status = "Low";
                color = YELLOW;
            } else {
                status = "Normal";
                color = GREEN;
            }

            // Print row with '|' separators; color only status text
            cout << "|" << left << setw(10) << p->getIDsp()
             << "|" << left << setw(30) << p->getNamesp()
             << "|" << left << setw(8) << qty
             << "|" << color << left << setw(8) << status << RESET << "|\n";
        }
    }
    // bottom border
    cout << "+" << string(totalWidth, '-') << "+\n";
}

// CSV export removed per user request.
