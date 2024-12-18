#ifndef ADMIN_H
#define ADMIN_H
#include "../headers/Person.h"
#include <vector>
#include <string>

class Admin : public Person {
private:

public:
    Admin(const std::string& firstName = "", const std::string& lastName = "",
          const std::string& password = "");
    void input() override;
    void display() const override;
    void bindToStatement(sqlite3_stmt* stmt, int& index) const override;
    void loadFromStatement(sqlite3_stmt* stmt) override;
};
#endif