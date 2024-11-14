#include "../headers/Specialist.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <iomanip>


Specialist::Specialist(const std::string& firstName, const std::string& lastName,
                       const std::string& password, const Address& address,
                       const std::string& contactInfo, const std::string& specialization,
                       const std::vector<std::string>& certifications)
    : Person(firstName, lastName, password), address(address), contactInfo(contactInfo),
      specialization(specialization), certifications(certifications) {}

void Specialist::input() {
    Person::input();
    address.input();
    std::cout << "Enter contact info: ";
    std::cin >> contactInfo;

    std::cout << "Enter specialization: ";
    std::getline(std::cin >> std::ws, specialization);  // use 'std::ws' to consume leading whitespace

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
    Person::display();
    address.display();
    std::cout << "Contact Info: " << contactInfo << std::endl;
    std::cout << "Specialization: " << specialization << std::endl;
    if (!certifications.empty()) {
        std::cout << "Certifications: ";
        for (const std::string& cert : certifications) {
            std::cout << cert << ", ";
        }
        std::cout << std::endl;
    }

    if (!ratings.empty()) {
        double sum = 0;
        for (int rating : ratings) sum += rating;
        std::cout << "Average rating: " << std::fixed << std::setprecision(1) << sum / ratings.size() << std::endl;
    }
}




void Specialist::bindToStatement(sqlite3_stmt* stmt, int& index) const {
    Person::bindToStatement(stmt, index);
    address.bindToStatement(stmt, index);
    sqlite3_bind_text(stmt, index++, contactInfo.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, index++, specialization.c_str(), -1, SQLITE_TRANSIENT);

    std::string certificationsStr;
    for (const auto& cert : certifications) {
        certificationsStr += cert + ",";
    }
    if (!certificationsStr.empty()) {
        certificationsStr.pop_back();
    }
    sqlite3_bind_text(stmt, index++, certificationsStr.c_str(), -1, SQLITE_TRANSIENT);


    std::string ratingsStr;
    for (int r : ratings) {
        ratingsStr += std::to_string(r) + ",";
    }
    if (!ratingsStr.empty()) {
        ratingsStr.pop_back();
    }

    sqlite3_bind_text(stmt, index++, ratingsStr.c_str(), -1, SQLITE_TRANSIENT);



    std::string reviewsStr;
    for (const auto& piece : reviews) {
        reviewsStr += piece + "\n";
    }
    if (!reviewsStr.empty()) reviewsStr.pop_back();

    sqlite3_bind_text(stmt, index++, reviewsStr.c_str(), -1, SQLITE_TRANSIENT);
}

void Specialist::loadFromStatement(sqlite3_stmt* stmt) {
    Person::loadFromStatement(stmt);
    address.loadFromStatement(stmt);

    int index = 11;
    contactInfo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
    specialization = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));


    const char* tempCertificationsStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
    std::string certificationsStr = (tempCertificationsStr != nullptr) ? tempCertificationsStr : "";
    certifications.clear();
    std::stringstream ssCert(certificationsStr);
    std::string cert;
    while (std::getline(ssCert, cert, ',')) {
        if (!cert.empty()) certifications.push_back(cert);
    }



    const char* tempRatingsStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
    std::string ratingsStr = (tempRatingsStr != nullptr) ? tempRatingsStr : "";
    ratings.clear();
    std::stringstream ssRatings(ratingsStr);
    std::string ratingPiece;
    while (std::getline(ssRatings, ratingPiece, ',')) {
        if (!ratingPiece.empty()) {
            try {
                ratings.push_back(std::stoi(ratingPiece));
            } catch (const std::invalid_argument& e) {

            } catch (const std::out_of_range& e) {

            }
        }
    }





    const char* tempReviewsStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index));
    std::string reviewsStr = (tempReviewsStr != nullptr) ? tempReviewsStr : "";

    reviews.clear();
    size_t start = 0;
    size_t end = reviewsStr.find('\n');
    while (end != std::string::npos) {
        reviews.push_back(reviewsStr.substr(start, end - start));
        start = end + 1;
        end = reviewsStr.find('\n', start);
    }
    if (start < reviewsStr.length()) {
        reviews.push_back(reviewsStr.substr(start));
    }
}


const Address& Specialist::getAddress() const { return address; }

void Specialist::addRating(int rating) { ratings.push_back(rating); }

void Specialist::addReview(const std::string& review) { reviews.push_back(review); }


double Specialist::getAverageRating() const {
    if (ratings.empty()) return 0;
    double sum = 0;
    for (int r : ratings) sum += r;
    return sum / ratings.size();
}

const std::vector<int>& Specialist::getRatings() const { return ratings; }

const std::vector<std::string>& Specialist::getReviews() const { return reviews; }

void Specialist::addCertification(const std::string& certification) { certifications.push_back(certification); }


std::string Specialist::getSpecialization() const { return specialization; }

const std::vector<std::string>& Specialist::getCertifications() const { return certifications; }

void Specialist::setSpecialization(const std::string& specialization) { this->specialization = specialization; }

std::string Specialist::getContactInfo() const { return contactInfo; }

void Specialist::setAddress(const Address& address) { this->address = address; }

void Specialist::setContactInfo(const std::string& contactInfo) { this->contactInfo = contactInfo; }