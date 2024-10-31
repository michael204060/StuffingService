
#ifndef USER_H
#define USER_H

#include "../headers/Person.h"
#include "../headers/Address.h"
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
    User(const std::string& firstName = "", const std::string& lastName = "",
         const std::string& password = "", const Address& address = Address(),
         const std::string& contactInfo = "", const std::vector<int>& ratings = {},
         const std::vector<std::string>& reviews = {});
    void input() override;
    void display() const override;
    void bindToStatement(sqlite3_stmt* stmt, int& index) const override;
    void loadFromStatement(sqlite3_stmt* stmt) override;

    void addRating(int rating);
    void addReview(const std::string& review);
    double getAverageRating() const;

    
    const Address& getAddress() const;
    std::string getContactInfo() const;
    const std::vector<int>& getRatings() const;
    const std::vector<std::string>& getReviews() const;

    
    void setAddress(const Address& address);
    void setContactInfo(const std::string& contactInfo);
};

#endif