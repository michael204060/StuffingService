#ifndef SPECIALIST_H
#define SPECIALIST_H

#include "User.h"
#include <vector>
#include <string>

class Specialist : public User {
private:
    std::string specialization;
    std::vector<std::string> certifications;

public:
    void input() override;
    void display() const override;

    void addCertification(const std::string& certification);
    void addRating(int rating); // Removed override
    void addReview(const std::string& review); // Removed override

    void bindToStatement(sqlite3_stmt* stmt, int &index) const override;
    void loadFromStatement(sqlite3_stmt* stmt) override;
};

#endif // SPECIALIST_H