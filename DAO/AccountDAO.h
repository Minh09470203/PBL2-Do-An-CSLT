#ifndef ACCOUNTDAO_H_INCLUDED
#define ACCOUNTDAO_H_INCLUDED
#include "IDAO.h"
#include "../DataStructure/MyVector.h"
#include "../Models/Account.h"

class AccountDAO : public VectorDAO<Account> {
public:
    AccountDAO(const string& file) : VectorDAO<Account>(file) {}

    bool create(const string &id, Account* entity) override;
    Account* read(const string &id) override; // here id is username
    bool update(const string &id, Account* entity) override;
    bool remove(const string &id) override; // username

    bool loadData() override;
    bool saveData() override;
};

#endif // ACCOUNTDAO_H_INCLUDED
