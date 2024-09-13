#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Person {
protected:
    string firstName;
    string lastName;
    string password;

public:
    virtual void input() = 0;
    virtual void display() const = 0;
    virtual ~Person() {}

    virtual void save(ofstream& outFile) const;
    virtual void load(ifstream& inFile);

    string getFirstName() const;
    string getLastName() const;
    bool checkPassword(const string& pass) const;
};

#endif // PERSON_H
