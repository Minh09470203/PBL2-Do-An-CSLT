#ifndef STAFFDAO_H_INCLUDED
#define STAFFDAO_H_INCLUDED
#include "IDAO.h"
#include "../DataStructure/MyVector.h"
#include "../Models/Staff.h"

class StaffDAO : public VectorDAO<Staff> {
public:
    StaffDAO(const string& file) : VectorDAO<Staff>(file) {}

    bool create(const string &id, Staff* entity) override;
    Staff* read(const string &id) override;
    bool update(const string &id, Staff* entity) override;
    bool remove(const string &id) override;

    bool loadData() override;
    bool saveData() override;
};

#endif // STAFFDAO_H_INCLUDED