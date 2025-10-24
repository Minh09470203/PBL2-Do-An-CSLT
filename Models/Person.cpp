#include "Person.h"

Person::Person(string id, string name, string phone)
    : ID(id), Name(name), Phone(phone) {}

Person::~Person() {}

string Person::getID() const { return ID; }
string Person::getName() const { return Name; }
string Person::getPhone() const { return Phone; }

void Person::setID(string id) { ID = id; }
void Person::setName(string name) { Name = name; }
void Person::setPhone(string phone) { Phone = phone; }

ostream& operator<<(ostream& os, const Person& person) {
    return person.output(os);
}

istream& operator>>(istream& is, Person& person) {
    return person.input(is);
}
