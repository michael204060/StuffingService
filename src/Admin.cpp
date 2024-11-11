#include "../headers/Admin.h"
#include <iostream>
#include <algorithm>
#include <sstream>

Admin::Admin(const std::string& firstName, const std::string& lastName, const std::string& password,
             const std::vector<std::string>& privileges)
    : Person(firstName, lastName, password), privileges(privileges) {}

void Admin::input() {
    Person::input();
    std::cout << "Enter number of privileges: ";
    size_t numPrivileges;
    std::cin >> numPrivileges;
    std::cin.ignore();
    privileges.resize(numPrivileges);
    for (size_t i = 0; i < numPrivileges; ++i) {
        std::cout << "Enter privilege #" << i + 1 << ": ";
        std::getline(std::cin, privileges[i]);
    }
}

void Admin::display() const {
    Person::display();
    std::cout << "Privileges: ";
    for (const std::string& privilege : privileges) {
        std::cout << privilege << ", ";
    }
    std::cout << std::endl;
}



void Admin::bindToStatement(sqlite3_stmt* stmt, int& index) const {
    Person::bindToStatement(stmt, index);
    std::string privilegesStr;
    for (const std::string& priv : privileges) {
        privilegesStr += priv + ",";
    }
    if (!privilegesStr.empty()) {
        privilegesStr.pop_back();
    }
    sqlite3_bind_text(stmt, index++, privilegesStr.c_str(), -1, SQLITE_TRANSIENT);
}

void Admin::loadFromStatement(sqlite3_stmt* stmt) {
    Person::loadFromStatement(stmt);
    int index = 4;

    const char* tempPrivilegesStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index));
    std::string privilegesStr = (tempPrivilegesStr != nullptr) ? tempPrivilegesStr : "";

    privileges.clear();
    std::stringstream ss(privilegesStr);
    std::string privilege;
    while (std::getline(ss, privilege, ',')) {
        if (!privilege.empty()) privileges.push_back(privilege);
    }
}


const std::vector<std::string>& Admin::getPrivileges() const { return privileges; }


template <typename T>
void Admin::addPrivilege(const T& privilege) {
    privileges.push_back(std::to_string(privilege));
}

template <typename T>
void Admin::removePrivilege(const T& privilege) {
    std::string strPrivilege = std::to_string(privilege);
    privileges.erase(std::remove(privileges.begin(), privileges.end(), strPrivilege), privileges.end());
}