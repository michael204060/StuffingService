#include "headers/Person.h"
#include <iostream>
void Person::input() {
    std::cout << "Enter first name: ";
    std::getline(std::cin, firstName);
    std::cout << "Enter last name: ";
    std::getline(std::cin, lastName);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
}
void Person::display() const {
    std::cout << "Profile: " << firstName << " " << lastName << std::endl;
}
std::string Person::getFirstName() const { return firstName; }
std::string Person::getLastName() const { return lastName; }
bool Person::checkPassword(const std::string& pass) const { return pass == password; }
std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << "First name: " << p.firstName << "\nLast name: " << p.lastName << std::endl;
    return os;
}
void Person::bindToStatement(sqlite3_stmt *stmt, int &index) const
{
    sqlite3_bind_text(stmt, index++, firstName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, lastName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, password.c_str(), -1, SQLITE_TRANSIENT);
}
void Person::loadFromStatement(sqlite3_stmt* stmt) {
    const char* tempFirstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    firstName = (tempFirstName) ? tempFirstName : "";  
    const char* tempLastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    lastName = (tempLastName) ? tempLastName : "";
    const char* tempPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    password = (tempPassword) ? tempPassword : "";
}
