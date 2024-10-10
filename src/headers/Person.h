#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>
#include <sqlite3.h>


class Person {
protected:
    std::string firstName;
    std::string lastName;
    std::string password;

public:
    virtual void input();
    virtual void display() const;
    virtual ~Person() {}


    virtual void bindToStatement(sqlite3_stmt* stmt, int& index) const = 0;
    virtual void loadFromStatement(sqlite3_stmt* stmt) = 0;


    bool operator==(const Person& other) const {
        return firstName == other.firstName && lastName == other.lastName;
    }
    bool operator!=(const Person& other) const { return !(*this == other); }

    bool operator<(const Person& other) const {
        return lastName < other.lastName || (lastName == other.lastName && firstName < other.firstName);
    }


    friend std::ostream& operator<<(std::ostream& os, const Person& person);


    std::string getFirstName() const;
    std::string getLastName() const;
    bool checkPassword(const std::string& pass) const;

};

std::ostream& operator<<(std::ostream& os, const Person& p);



#endif // PERSON_H