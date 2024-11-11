#ifndef ADDRESS_H
#define ADDRESS_H
#include <string>
#include <sqlite3.h>
class Address {
private:
    std::string country;
    std::string region;
    std::string city;
    std::string street;
    std::string house;
    std::string apartment;
public:
    Address(); 
    Address(const std::string& country, const std::string& region, const std::string& city,
        const std::string& street, const std::string& house, const std::string& apartment);
    void input();
    void display() const;
    void bindToStatement(sqlite3_stmt* stmt, int& index) const;
    void loadFromStatement(sqlite3_stmt* stmt);
    std::string getCountry() const;
    std::string getRegion() const;
    std::string getCity() const;
    std::string getStreet() const;
    std::string getHouse() const;
    std::string getApartment() const;
    void setCountry(const std::string& country);
    void setRegion(const std::string& region);
    void setCity(const std::string& city);
    void setStreet(const std::string& street);
    void setHouse(const std::string& house);
    void setApartment(const std::string& apartment);
};
#endif 