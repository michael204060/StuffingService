#ifndef ADDRESS_H
#define ADDRESS_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Address {
    string country;
    string region;
    string city;
    string street;
    string house;
    string apartment;

    void input();
    void display() const;
    void save(ofstream& outFile) const;
    void load(ifstream& inFile);
};

#endif // ADDRESS_H
