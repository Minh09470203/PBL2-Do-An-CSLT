#ifndef PERSON_H_INCLUDED
#define PERSON_H_INCLUDED
#include <iostream>
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
    

    string getID() const;
    string getName() const;
    string getPhone() const;

    void setID(string id);
    void setName(string name);
    void setPhone(string phone);
    virtual ostream& output(ostream& os) const = 0;
    friend ostream& operator<<(ostream& os, const Person& person);
    virtual istream& input(istream& is) = 0;
    friend istream& operator>>(istream& is, Person& person);
};

#endif // PERSON_H_INCLUDED