    
#ifndef SPECIALIST_H
#define SPECIALIST_H

#include "headers/User.h"
#include <vector>
#include <string>

class Specialist : public User {
private:
    std::string specialization;
    std::vector<std::string> certifications;

public:
    Specialist(const std::string& firstName = "", const std::string& lastName = "",
              const std::string& password = "", const Address& address = Address(),
              const std::string& contactInfo = "", const std::string& specialization = "",
              const std::vector<std::string>& certifications = {});
    void input() override;
    void display() const override;
    void bindToStatement(sqlite3_stmt* stmt, int& index) const override;
    void loadFromStatement(sqlite3_stmt* stmt) override;

    void addCertification(const std::string& certification);

    
    std::string getSpecialization() const;
    const std::vector<std::string>& getCertifications() const;

    
    void setSpecialization(const std::string& specialization);
};

#endif