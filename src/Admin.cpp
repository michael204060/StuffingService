#include "../headers/Admin.h"
#include <iostream>
#include <algorithm>

Admin::Admin(const std::string& firstName, const std::string& lastName, const std::string& password)
    : Person(firstName, lastName, password) {}

void Admin::input() {
    Person::input();
}

void Admin::display() const {
    Person::display();
}

void Admin::bindToStatement(sqlite3_stmt* stmt, int& index) const {
    Person::bindToStatement(stmt, index);
}

void Admin::loadFromStatement(sqlite3_stmt* stmt) {
    Person::loadFromStatement(stmt);
}