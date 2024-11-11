#ifndef SPECIALIST_H
#define SPECIALIST_H

#include "../headers/Person.h"
#include <vector>
#include <string>
#include "Address.h"

class Specialist : public Person {
private:
    Address address;
    std::string contactInfo;
    std::string specialization;
    std::vector<std::string> certifications;
    std::vector<int> ratings;
    std::vector<std::string> reviews;

public:
    Specialist(const std::string& firstName = "", const std::string& lastName = "",
              const std::string& password = "", const Address& address = Address(),
              const std::string& contactInfo = "", const std::string& specialization = "",
              const std::vector<std::string>& certifications = {});
    void input() override;
    void display() const override;
    void bindToStatement(sqlite3_stmt* stmt, int& index) const override;
    void loadFromStatement(sqlite3_stmt* stmt) override;
    const Address& getAddress() const override;


    void addRating(int rating);
    void addReview(const std::string& review);
    double getAverageRating() const;
    const std::vector<int>& getRatings() const;
    const std::vector<std::string>& getReviews() const;


    void addCertification(const std::string& certification);
    std::string getSpecialization() const;
    const std::vector<std::string>& getCertifications() const;
    void setSpecialization(const std::string& specialization);
    std::string getContactInfo() const;
    void setAddress(const Address& address);
    void setContactInfo(const std::string& contactInfo);
};

#endif