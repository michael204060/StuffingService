#include "headers/Admin.h"
#include <iostream>
#include <algorithm>
#include <sstream>

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
    for (const std::string& privilege : privileges) std::cout << privilege << ", ";
    std::cout << std::endl;
}

void Admin::addPrivilege(const std::string& privilege) { privileges.push_back(privilege); }
void Admin::removePrivilege(const std::string& privilege) { privileges.erase(std::remove(privileges.begin(), privileges.end(), privilege), privileges.end()); }

void Admin::bindToStatement(sqlite3_stmt* stmt, int &index) const {
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

// void Admin::loadFromStatement(sqlite3_stmt* stmt) {
//     Person::loadFromStatement(stmt);
//     std::string privilegesStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 14));
//     privileges.clear();
//     std::stringstream ss(privilegesStr);
//     std::string privilege;
//     while (std::getline(ss, privilege, ',')) {
//         if (!privilege.empty()) privileges.push_back(privilege);
//     }
// }

void Admin::loadFromStatement(sqlite3_stmt* stmt) {
    Person::loadFromStatement(stmt);
    const char *privilegesValue = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 14));
    if (privilegesValue != nullptr) {
        std::string privilegesStr(privilegesValue);
        privileges.clear();
        std::stringstream ss(privilegesStr);
        std::string privilege;
        while (std::getline(ss, privilege, ',')) {
            if (!privilege.empty()) privileges.push_back(privilege);
        }
    }
}
