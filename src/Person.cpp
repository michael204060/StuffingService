
#include "../headers/Person.h"
#include <iostream>

Person::Person(const std::string& firstName, const std::string& lastName, const std::string& password)
    : firstName(firstName), lastName(lastName), password(password) {}

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
std::string Person::getPassword() const { return password; }

void Person::setFirstName(const std::string& firstName) { this->firstName = firstName; }
void Person::setLastName(const std::string& lastName) { this->lastName = lastName; }
void Person::setPassword(const std::string& password) { this->password = password; }

bool Person::checkPassword(const std::string& pass) const { return pass == password; }

bool Person::operator==(const Person& other) const {
    return firstName == other.firstName && lastName == other.lastName;
}

bool Person::operator!=(const Person& other) const { return !(*this == other); }

bool Person::operator<(const Person& other) const {
    return lastName < other.lastName || (lastName == other.lastName && firstName < other.firstName);
}

std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << "First name: " << p.firstName << "\nLast name: " << p.lastName << std::endl;
    return os;
}

void Person::bindToStatement(sqlite3_stmt* stmt, int& index) const {
    sqlite3_bind_text(stmt, index++, firstName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, lastName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, password.c_str(), -1, SQLITE_TRANSIENT);
}

void Person::loadFromStatement(sqlite3_stmt* stmt) {
    int index = 2; 

    const char* tempFirstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
    firstName = (tempFirstName != nullptr) ? tempFirstName : "";

    const char* tempLastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
    lastName = (tempLastName != nullptr) ? tempLastName : "";

    const char* tempPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index));
    password = (tempPassword != nullptr) ? tempPassword : "";
}
const Address& Person::getAddress() const {
    static Address emptyAddress; 
    return emptyAddress;        
}
