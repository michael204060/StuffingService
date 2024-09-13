#ifndef USER_H
#define USER_H

#include "Person.h"
#include <vector>
#include <string>
#include <iostream>
#include "Address.h"
#include <fstream>

using namespace std;

class User : public Person {
private:
    Address address;
    string contactInfo;
    vector<int> ratings;
    vector<string> reviews;

public:
    void input() override;
    void display() const override;

    void addRating(int rating);
    void addReview(const string& review);
    double getAverageRating() const;

    void save(ofstream& outFile) const override;
    void load(ifstream& inFile) override;
};

#endif // USER_H
