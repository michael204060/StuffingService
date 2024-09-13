#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Admin : public Person {
    vector<string> privileges;

public:
    void input() override;
    void display() const override;

    void addPrivilege(const string& privilege);
    void removePrivilege(const string& privilege);

    void save(ofstream& outFile) const override;
    void load(ifstream& inFile) override;
};

#endif // ADMIN_H
