#ifndef STAFF_H_INCLUDED
#define STAFF_H_INCLUDED
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "../DataStructure/MyVector.h"
#include "Person.h"
using namespace std;

// Forward declare DAO used in function prototypes to avoid circular include
class StaffDAO;

class Staff : public Person {
private:
    string Position;
public:
    Staff(
          string ID_NV = "",
          string Name_NV = "",
          string Position = "",
          string SDT = ""
    )
    : Person(ID_NV, Name_NV, SDT), Position(Position) {}

    // Override virtual methods for polymorphism
    ostream& output(ostream& os) const override;
    istream& input(istream& is) override;

    string getIDnv();
    string getNamenv();
    string getPosition();
    void setPosition(string newPosition);
};

// Function declarations (implemented in Staff.cpp)
void printStaffList(StaffDAO &staffDAO);
bool isduplicateStaff(StaffDAO &staffDAO, const string& id);
void StaffInsert(StaffDAO &staffDAO);
void StaffDelete(StaffDAO &staffDAO);
#endif // STAFF_H_INCLUDED
