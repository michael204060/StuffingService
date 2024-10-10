#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include <vector>
#include <string>

class Admin : public Person {
private:
    std::vector<std::string> privileges;

public:
    void input() override;
    void display() const override;

    void addPrivilege(const std::string& privilege);
    void removePrivilege(const std::string& privilege);

    void bindToStatement(sqlite3_stmt* stmt, int &index) const override;
    void loadFromStatement(sqlite3_stmt* stmt) override;
};

#endif // ADMIN_H

