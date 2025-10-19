#ifndef PERSON_H_INCLUDED
#define PERSON_H_INCLUDED
#include <string>
#include "../DataStructure/MyVector.h"
using namespace std;

class Person {
protected:
    string ID;
    string Name;
    string Phone;
    
public:
    Person(string id = "", string name = "", string phone = "");

    virtual ~Person();
    virtual void display() const = 0;

    string getID() const;
    string getName() const;
    string getPhone() const;

    void setID(string id);
    void setName(string name);
    void setPhone(string phone);
};

#endif // PERSON_H_INCLUDED