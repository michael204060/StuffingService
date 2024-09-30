#include "headers/Address.h"

void Address::input() {
    cout << "Enter country: ";
    getline(cin, country);
    cout << "Enter region: ";
    getline(cin, region);
    cout << "Enter city: ";
    getline(cin, city);
    cout << "Enter street: ";
    getline(cin, street);
    cout << "Enter house: ";
    getline(cin, house);
    cout << "Enter apartment: ";
    getline(cin, apartment);
}

void Address::display() const {
    cout << "Address: " << country << ", " << region << ", " << city;
    if (!street.empty()) cout << ", " << street;
    if (!house.empty()) cout << ", House " << house;
    if (!apartment.empty()) cout << ", Apt " << apartment;
    cout << endl;
}

void Address::save(ofstream& outFile) const {
    size_t size;
    size = country.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(country.c_str(), size);
    size = region.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(region.c_str(), size);
    size = city.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(city.c_str(), size);
    size = street.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(street.c_str(), size);
    size = house.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(house.c_str(), size);
    size = apartment.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(apartment.c_str(), size);
}

void Address::load(ifstream& inFile) {
    size_t size;
    char* buffer;

    inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; country = buffer; delete[] buffer;
    inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; region = buffer; delete[] buffer;
    inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; city = buffer; delete[] buffer;
    inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; street = buffer; delete[] buffer;
    inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; house = buffer; delete[] buffer;
    inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; apartment = buffer; delete[] buffer;
}
