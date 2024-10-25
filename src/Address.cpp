
#include "headers/Address.h"
#include <iostream>

Address::Address() : country(""), region(""), city(""), street(""), house(""), apartment("") {}

Address::Address(const std::string& country, const std::string& region, const std::string& city,
    const std::string& street, const std::string& house, const std::string& apartment)
    : country(country), region(region), city(city),
    street(street), house(house), apartment(apartment) {}

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
    int index = 5; 
    country = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
    region = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
    city = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
    street = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
    house = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
    apartment = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index));
}


std::string Address::getCountry() const { return country; }
std::string Address::getRegion() const { return region; }
std::string Address::getCity() const { return city; }
std::string Address::getStreet() const { return street; }
std::string Address::getHouse() const { return house; }
std::string Address::getApartment() const { return apartment; }


void Address::setCountry(const std::string& country) { this->country = country; }
void Address::setRegion(const std::string& region) { this->region = region; }
void Address::setCity(const std::string& city) { this->city = city; }
void Address::setStreet(const std::string& street) { this->street = street; }
void Address::setHouse(const std::string& house) { this->house = house; }
void Address::setApartment(const std::string& apartment) { this->apartment = apartment; }