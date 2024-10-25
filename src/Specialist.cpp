
#include "headers/Specialist.h"
#include <iostream>
#include <sstream>
#include <limits>

Specialist::Specialist(const std::string& firstName, const std::string& lastName, 
                     const std::string& password, const Address& address,
                     const std::string& contactInfo, const std::string& specialization,
                     const std::vector<std::string>& certifications)
    : User(firstName, lastName, password, address, contactInfo), 
      specialization(specialization), certifications(certifications) {}

void Specialist::input() {
    User::input();
    std::cout << "Enter specialization: ";
    std::getline(std::cin, specialization);
    int numCertifications;
    std::cout << "Enter the number of certifications: ";
    std::cin >> numCertifications;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    certifications.resize(numCertifications);
    for (int i = 0; i < numCertifications; ++i) {
        std::cout << "Enter certification #" << i + 1 << ": ";
        std::getline(std::cin, certifications[i]);
    }
}

void Specialist::display() const {
    User::display();
    std::cout << "Specialization: " << specialization << std::endl;
    if (!certifications.empty()) {
        std::cout << "Certifications: ";
        for (const std::string& cert : certifications) std::cout << cert << ", ";
        std::cout << std::endl;
    }
}

void Specialist::addCertification(const std::string& certification) {
    certifications.push_back(certification);
}

void Specialist::bindToStatement(sqlite3_stmt* stmt, int& index) const {
    User::bindToStatement(stmt, index);
    sqlite3_bind_text(stmt, index++, specialization.c_str(), -1, SQLITE_TRANSIENT);
    std::string certificationsStr;
    for (const auto& cert : certifications) {
        certificationsStr += cert + ",";
    }
    if (!certificationsStr.empty()) {
        certificationsStr.pop_back();
    }
    sqlite3_bind_text(stmt, index++, certificationsStr.c_str(), -1, SQLITE_TRANSIENT);
}

void Specialist::loadFromStatement(sqlite3_stmt* stmt) {
    User::loadFromStatement(stmt);
    int index = 12; 

    const char* tempSpecialization = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
    specialization = (tempSpecialization != nullptr) ? tempSpecialization : "";

    const char* tempCertificationsStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index));
    std::string certificationsStr = (tempCertificationsStr != nullptr) ? tempCertificationsStr : "";

    certifications.clear();
    std::stringstream ss(certificationsStr);
    std::string cert;
    while (std::getline(ss, cert, ',')) {
        if (!cert.empty()) certifications.push_back(cert);
    }
}


std::string Specialist::getSpecialization() const { return specialization; }
const std::vector<std::string>& Specialist::getCertifications() const { return certifications; }
void Specialist::setSpecialization(const std::string& specialization) { this->specialization = specialization; }
