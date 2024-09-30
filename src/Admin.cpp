#include "headers/Admin.h"
#include <algorithm>

void Admin::input() {
    cout << "Enter first name: ";
    getline(cin, firstName);
    cout << "Enter last name: ";
    getline(cin, lastName);
    cout << "Enter password: ";
    getline(cin, password);
    cout << "Enter number of privileges: ";
    size_t numPrivileges;
    cin >> numPrivileges;
    cin.ignore();
    privileges.resize(numPrivileges);
    for (size_t i = 0; i < numPrivileges; ++i) {
        cout << "Enter privilege #" << i + 1 << ": ";
        getline(cin, privileges[i]);
    }
}

void Admin::display() const {
    cout << "Admin Profile: " << firstName << " " << lastName << endl;
    cout << "Privileges: ";
    for (const string& privilege : privileges) cout << privilege << ", ";
    cout << endl;
}

void Admin::addPrivilege(const string& privilege) { privileges.push_back(privilege); }
void Admin::removePrivilege(const string& privilege) { privileges.erase(remove(privileges.begin(), privileges.end(), privilege), privileges.end()); }

void Admin::save(ofstream& outFile) const {
    outFile.write("A", 1);
    Person::save(outFile);

    size_t size = privileges.size(); outFile.write((char*)&size, sizeof(size));
    for (const string& privilege : privileges) {
        size = privilege.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(privilege.c_str(), size);
    }
}

void Admin::load(ifstream& inFile) {
    Person::load(inFile);
    size_t size;
    char* buffer;

    inFile.read((char*)&size, sizeof(size));
    privileges.resize(size);
    for (size_t i = 0; i < size; ++i) {
        inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; privileges[i] = buffer; delete[] buffer;
    }
}
