
#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>
#include <sqlite3.h>
#include "headers/Address.h"

class Person {
private:
    std::string firstName;
    std::string lastName;
    std::string password;

public:
    Person(const std::string& firstName = "", const std::string& lastName = "", const std::string& password = "");
    virtual ~Person() = default;
    virtual void input();
    virtual void display() const;
    virtual void bindToStatement(sqlite3_stmt* stmt, int& index) const;
    virtual void loadFromStatement(sqlite3_stmt* stmt);

    
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getPassword() const;
    virtual const Address& getAddress() const;
    
    void setFirstName(const std::string& firstName);
    void setLastName(const std::string& lastName);
    void setPassword(const std::string& password);

    bool checkPassword(const std::string& pass) const;

    bool operator==(const Person& other) const;
    bool operator!=(const Person& other) const;
    bool operator<(const Person& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Person& person);
};

std::ostream& operator<<(std::ostream& os, const Person& p);

#endif 