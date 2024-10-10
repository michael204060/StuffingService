#ifndef USER_H
#define USER_H
#include "Person.h"
#include "Address.h"
#include <vector>
#include <string>
#include <iostream>
class User : public Person {
private:
    Address address;
    std::string contactInfo;
    std::vector<int> ratings;
    std::vector<std::string> reviews;
public:
    void input() override;
    void display() const override;
    void addRating(int rating);
    void addReview(const std::string& review);
    double getAverageRating() const;
    void bindToStatement(sqlite3_stmt* stmt, int &index) const override;
    void loadFromStatement(sqlite3_stmt* stmt) override;
};
#endif