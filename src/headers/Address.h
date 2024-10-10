#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <sqlite3.h>

class Address {
public:
    std::string country;
    std::string region;
    std::string city;
    std::string street;
    std::string house;
    std::string apartment;

    void input();
    void display() const;
    void bindToStatement(sqlite3_stmt* stmt, int& index) const;
    void loadFromStatement(sqlite3_stmt* stmt);
};

#endif // ADDRESS_H
