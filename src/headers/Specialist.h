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
    void addRating(int rating); 
    void addReview(const std::string& review); 
    void bindToStatement(sqlite3_stmt* stmt, int &index) const override;
    void loadFromStatement(sqlite3_stmt* stmt) override;
};
#endif 