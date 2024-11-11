#ifndef ADMIN_H
#define ADMIN_H
#include "../headers/Person.h"
#include <vector>
#include <string>
#include <sstream> 
class Admin : public Person {
private:
    std::vector<std::string> privileges;
public:
    Admin(const std::string& firstName = "", const std::string& lastName = "",
          const std::string& password = "", const std::vector<std::string>& privileges = {});
    void input() override;
    void display() const override;
    void bindToStatement(sqlite3_stmt* stmt, int& index) const override;
    void loadFromStatement(sqlite3_stmt* stmt) override;
    template <typename T>
    void addPrivilege(const T& privilege);  
    template <typename T>
    void removePrivilege(const T& privilege); 
    const std::vector<std::string>& getPrivileges() const;
};
template<typename T>
class PrivilegeManager {
private:
    std::vector<T> privileges;
public:
    void addPrivilege(const T& privilege) {
        privileges.push_back(privilege);
    }
    void removePrivilege(const T& privilege) {
        privileges.erase(std::remove(privileges.begin(), privileges.end(), privilege), privileges.end());
    }
    const std::vector<std::string>& getPrivileges() const;
};
#endif