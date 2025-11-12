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
    const int maxBar = 40; // maximum bar width in characters (keep short to avoid wrap)
    for (int m = 1; m <= 12; ++m) {
        unsigned long rev = months[m];
        int barLen = 0;
        if (maxRev > 0) barLen = (int)((__int128)rev * maxBar / maxRev); // avoid overflow
        if (rev > 0 && barLen == 0) barLen = 1; // ensure visible mark for small non-zero values
        if (barLen > maxBar) barLen = maxBar;
    // build bar string using block character '█' (UTF-8)
    string bar;
    for (int i = 0; i < barLen; ++i) bar += "█";
    // print line: Month X  [bar]  <amount> VND (formatted)
    cout << "Month " << setw(2) << m << " ";
    if (!bar.empty()) cout << bar << " ";
    cout << formatNumber(rev) << " VND\n" << endl;
    }

    // --- Analysis section (cleaner, multi-line for readability) ---
    unsigned long totalYear = 0;
    for (int i = 1; i <= 12; ++i) totalYear += months[i];

    // helper to sum a year without extra globals
    auto sumYear = [&](int y) -> unsigned long {
        unsigned long tmp[13];
        revenueByMonth(y, tmp);
        unsigned long s = 0;
        for (int i = 1; i <= 12; ++i) s += tmp[i];
        return s;
    };

    unsigned long totalPrev = sumYear(year - 1);
    unsigned long totalPrevPrev = sumYear(year - 2);

    int maxMonth = -1, minMonth = -1;
    unsigned long maxVal = 0;
    unsigned long minVal = (unsigned long)-1;
    for (int i = 1; i <= 12; ++i) {
        unsigned long v = months[i];
        if (maxMonth == -1 || v > maxVal) { maxVal = v; maxMonth = i; }
        if (minMonth == -1 || v < minVal) { minVal = v; minMonth = i; }
    }

    const int aWidth = 60;
    const string RESET = "\033[0m";
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";

    unsigned long avgMonth = totalYear / 12;

    unsigned long forecastFromPrev = 0;
    if (totalPrevPrev > 0 && totalPrev > 0) {
        double growthRatePrev = ((double)totalPrev - (double)totalPrevPrev) / (double)totalPrevPrev;
        double forecast_d = (double)totalPrev * (1.0 + growthRatePrev);
        if (forecast_d < 0) forecast_d = 0;
        forecastFromPrev = (unsigned long)(forecast_d + 0.5);
    }

    unsigned long forecastNext = 0;
    if (totalPrev > 0) {
        double growthRateCurr = ((double)totalYear - (double)totalPrev) / (double)totalPrev;
        double fnext_d = (double)totalYear * (1.0 + growthRateCurr);
        if (fnext_d < 0) fnext_d = 0;
        forecastNext = (unsigned long)(fnext_d + 0.5);
    }

    cout << "+" << string(aWidth, '-') << "+\n";
    string title = "ANALYSIS";
    int leftPad = (aWidth - (int)title.size()) / 2;
    int rightPad = aWidth - (int)title.size() - leftPad;
    cout << "|" << string(leftPad, ' ') << title << string(rightPad, ' ') << "|\n";
    cout << "+" << string(aWidth, '-') << "+\n";

    // Total revenue + forecast comparison
    string totalLine = string("Total revenue: ") + formatNumber(totalYear) + " VND";
    if (forecastFromPrev > 0) {
        double pct = ((double)totalYear - (double)forecastFromPrev) * 100.0 / (double)forecastFromPrev;
        char buf[64];
        sprintf(buf, " (%+.2f%% vs forecast)    ", pct);
        string pctStr(buf);
        string colored = (pct >= 0.0) ? (GREEN + pctStr + RESET) : (RED + pctStr + RESET);
        totalLine += colored;
    } else {
        totalLine += string(" (no prior forecast)");
    }
    cout << "| " << left << setw(aWidth - 4) << totalLine << "   |\n";

    // Average and forecast next year
    cout << "| " << left << setw(aWidth - 4) << (string("Average month: ") + formatNumber(avgMonth) + " VND") << "   |\n";
    if (forecastNext > 0) cout << "| " << left << setw(aWidth - 4) << (string("Forecast next year: ") + formatNumber(forecastNext) + " VND") << "   |\n";

    // Growth vs previous year
    if (totalPrev == 0) {
        cout << "| " << left << setw(aWidth - 4) << (string("Growth Vs ") + to_string(year - 1) + ": N/A (no data)") << "   |\n";
    } else {
        long long diff = (long long)totalYear - (long long)totalPrev;
        double pct = ((double)diff) * 100.0 / (double)totalPrev;
        char buf2[64];
        sprintf(buf2, "Growth Vs %d: %.2f%%", year - 1, pct);
        string pctLine(buf2);
        string colored = (pct >= 0.0) ? (GREEN + pctLine + RESET) : (RED + pctLine + RESET);
        cout << "| " << left << setw(aWidth - 4) << colored << "            |\n";
    }

    // Highest / lowest months
    if (maxMonth == -1) {
        cout << "| " << left << setw(aWidth - 4) << string("No revenue data for this year.") << "   |\n";
    } else {
        string highLine = string("Highest month: ") + (maxMonth < 10 ? string("0") + to_string(maxMonth) : to_string(maxMonth)) + string(" (") + formatNumber(maxVal) + string(" VND)");
        cout << "| " << left << setw(aWidth - 4) << highLine << "   |\n";
        string lowLine = string("Lowest month:  ") + (minMonth < 10 ? string("0") + to_string(minMonth) : to_string(minMonth)) + string(" (") + formatNumber(minVal) + string(" VND)");
        cout << "| " << left << setw(aWidth - 4) << lowLine << "   |\n";
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
    // Simple, self-contained aggregation using MyVector only
    struct Agg { string id; string name; unsigned long qty; unsigned long revenue; };
    MyVector<Agg> vec;

    MyVector<Invoice*>& invs = invoiceDao->getDataCache();
    for (int i = 0; i < invs.getSize(); ++i) {
        Invoice* inv = invs[i];
        for (int j = 0; j < inv->getDetails().getSize(); ++j) {
            OrderDetail* od = inv->getDetails()[j];
            if (!od || !od->getProduct()) continue;
            string pid = od->getProduct()->getIDsp();
            unsigned long q = od->getQuantity();
            unsigned long r = (unsigned long)q * od->getUnitPrice();
            int idx = -1;
            for (int k = 0; k < vec.getSize(); ++k) if (vec[k].id == pid) { idx = k; break; }
            if (idx == -1) {
                Agg a; a.id = pid; a.name = pid; a.qty = q; a.revenue = r; vec.Push_back(a);
            } else {
                vec[idx].qty += q; vec[idx].revenue += r;
            }
        }
    }

    // fill names
    for (int i = 0; i < vec.getSize(); ++i) {
        Product* p = productDao->read(vec[i].id);
        if (p) vec[i].name = p->getNamesp();
    }

    int total = vec.getSize();
    int limit = (N < total) ? N : total;
    cout << "\nTop " << limit << " products by quantity:\n";
    cout << left << setw(10) << "ID" << setw(30) << "Name" << setw(10) << "Qty" << setw(15) << "Revenue" << "\n";
    cout << string(70, '-') << "\n";
    for (int k = 0; k < limit; ++k) {
        int best = k;
        for (int j = k+1; j < total; ++j) if (vec[j].qty > vec[best].qty) best = j;
        if (best != k) { Agg tmp = vec[k]; vec[k] = vec[best]; vec[best] = tmp; }
        cout << left << setw(10) << vec[k].id << setw(30) << vec[k].name << setw(10) << vec[k].qty << setw(15) << formatNumber(vec[k].revenue) << "\n";
    }
}

// Implement top customers by revenue
void StatsService::printTopCustomersByRevenue(int N) {
    struct Cust { string id; string name; unsigned long amount; };
    MyVector<Cust> vec;

    MyVector<Invoice*>& invs = invoiceDao->getDataCache();
    for (int i = 0; i < invs.getSize(); ++i) {
        Invoice* inv = invs[i];
        if (!inv->getCustomer()) continue;
        string cid = inv->getCustomer()->getID();
        unsigned long amt = inv->getTotalAmount();
        int idx = -1;
        for (int k = 0; k < vec.getSize(); ++k) if (vec[k].id == cid) { idx = k; break; }
        if (idx == -1) { Cust c; c.id = cid; c.name = cid; c.amount = amt; vec.Push_back(c); }
        else vec[idx].amount += amt;
    }

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
        for (int j = k+1; j < total; ++j) if (vec[j].amount > vec[best].amount) best = j;
        if (best != k) { Cust tmp = vec[k]; vec[k] = vec[best]; vec[best] = tmp; }
        cout << left << setw(10) << vec[k].id << setw(30) << vec[k].name << setw(15) << formatNumber(vec[k].amount) << "\n";
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
