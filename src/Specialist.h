#ifndef SPECIALIST_H
#define SPECIALIST_H

#include "User.h"
#include <vector>

class Specialist : public User {
    string specialization;
    vector<string> certifications;

public:
    void input() override;
    void display() const override;

    void addCertification(const string& certification);

    void save(ofstream& outFile) const override;
    void load(ifstream& inFile) override;
};

#endif // SPECIALIST_H
