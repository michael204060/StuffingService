#include "headers/Person.h"

void Person::save(ofstream& outFile) const {
    size_t size;
    size = firstName.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(firstName.c_str(), size);
    size = lastName.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(lastName.c_str(), size);
    size = password.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(password.c_str(), size);
}

void Person::load(ifstream& inFile) {

    size_t size;
    char* buffer;
    inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; firstName = buffer; delete[] buffer;

    inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; lastName = buffer; delete[] buffer;
    inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; password = buffer; delete[] buffer;
}

string Person::getFirstName() const { return firstName; }
string Person::getLastName() const { return lastName; }
bool Person::checkPassword(const string& pass) const { return pass == password; }
