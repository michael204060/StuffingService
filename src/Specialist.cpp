#include "headers/Specialist.h"

void Specialist::input() {
    User::input();
    cout << "Enter specialization: ";
    getline(cin, specialization);
}

void Specialist::display() const {
    User::display();
    cout << "Specialization: " << specialization << endl;
    if (!certifications.empty()) {
        cout << "Certifications: ";
        for (const string& cert : certifications) cout << cert << ", ";
        cout << endl;
    }
}

void Specialist::addCertification(const string& certification) {
    certifications.push_back(certification);
}

void Specialist::save(ofstream& outFile) const {
    outFile.write("S", 1);
    User::save(outFile);

    size_t size = specialization.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(specialization.c_str(), size);
    size = certifications.size(); outFile.write((char*)&size, sizeof(size));
    for (const string& cert : certifications) {
        size = cert.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(cert.c_str(), size);
    }
}

void Specialist::load(ifstream& inFile) {
    User::load(inFile);
    size_t size;
    char* buffer;

    inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; specialization = buffer; delete[] buffer;

    inFile.read((char*)&size, sizeof(size));
    certifications.resize(size);
    for (size_t i = 0; i < size; ++i) {
        inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; certifications[i] = buffer; delete[] buffer;
    }
}
