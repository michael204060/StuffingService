#include "headers/Address.h"
#include <iostream>

void Address::input() {
    std::cout << "Enter country: ";
    std::getline(std::cin, country);
    std::cout << "Enter region: ";
    std::getline(std::cin, region);
    std::cout << "Enter city: ";
    std::getline(std::cin, city);
    std::cout << "Enter street: ";
    std::getline(std::cin, street);
    std::cout << "Enter house: ";
    std::getline(std::cin, house);
    std::cout << "Enter apartment: ";
    std::getline(std::cin, apartment);
}

void Address::display() const {
    std::cout << "Address: " << country << ", " << region << ", " << city;
    if (!street.empty()) std::cout << ", " << street;
    if (!house.empty()) std::cout << ", House " << house;
    if (!apartment.empty()) std::cout << ", Apt " << apartment;
    std::cout << std::endl;
}

void Address::bindToStatement(sqlite3_stmt* stmt, int& index) const {
    sqlite3_bind_text(stmt, index++, country.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, region.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, city.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, street.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, house.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, apartment.c_str(), -1, SQLITE_TRANSIENT);
}
void Address::loadFromStatement(sqlite3_stmt* stmt) {
    const char *countryValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    if (countryValue != nullptr) {
        country = countryValue;
    }

    const char *regionValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    if (regionValue != nullptr) {
        region = regionValue;
    }

    const char *cityValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
    if (cityValue != nullptr) {
        city = cityValue;
    }

    const char *streetValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
    if (streetValue != nullptr) {
        street = streetValue;
    }

    const char *houseValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
    if (houseValue != nullptr) {
        house = houseValue;
    }

    const char *apartmentValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10))   ;
    if (apartmentValue != nullptr) {
        apartment = apartmentValue;
    }
}



