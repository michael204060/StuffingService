// User.cpp
#include "../headers/User.h"
#include <iomanip>
#include <sstream>

User::User(const std::string& firstName, const std::string& lastName, const std::string& password,
           const Address& address, const std::string& contactInfo, const std::vector<int>& ratings,
           const std::vector<std::string>& reviews)
    : Person(firstName, lastName, password), address(address), contactInfo(contactInfo), 
      ratings(ratings), reviews(reviews) {}

void User::input() {
    Person::input();
    address.input();
    std::cout << "Enter contact info: ";
    std::getline(std::cin, contactInfo);
}

void User::display() const {
    Person::display();
    address.display();
    std::cout << "Contact info: " << contactInfo << std::endl;
    if (!ratings.empty()) {
        double sum = 0;
        for (int rating : ratings) sum += rating;
        std::cout << "Average rating: " << std::fixed << std::setprecision(1) << sum / ratings.size() << std::endl;
    }
}

void User::addRating(int rating) { 
    ratings.push_back(rating); 
}

void User::addReview(const std::string& review) { 
    reviews.push_back(review); 
}

double User::getAverageRating() const {
    if (ratings.empty()) return 0;
    double sum = 0;
    for (int r : ratings) sum += r;
    return sum / ratings.size();
}

void User::bindToStatement(sqlite3_stmt* stmt, int& index) const {
    Person::bindToStatement(stmt, index);
    address.bindToStatement(stmt, index);
    sqlite3_bind_text(stmt, index++, contactInfo.c_str(), -1, SQLITE_TRANSIENT);
    std::string ratingsStr;
    for (int r : ratings) {
        ratingsStr += std::to_string(r) + ",";
    }
    if (!ratingsStr.empty()) ratingsStr.pop_back();
    sqlite3_bind_text(stmt, index++, ratingsStr.c_str(), -1, SQLITE_TRANSIENT);
    std::string reviewsStr;
    for (const auto& piece : reviews) {
        reviewsStr += piece + "\n";
    }
    if (!reviewsStr.empty()) reviewsStr.pop_back();
    sqlite3_bind_text(stmt, index++, reviewsStr.c_str(), -1, SQLITE_TRANSIENT);
}

void User::loadFromStatement(sqlite3_stmt* stmt) {
    Person::loadFromStatement(stmt);
    address.loadFromStatement(stmt);
    int index = 11;

    const char* tempContactInfo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
    contactInfo = (tempContactInfo != nullptr) ? tempContactInfo : "";

    const char* tempRatingsStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++));
    std::string ratingsStr = (tempRatingsStr != nullptr) ? tempRatingsStr : "";
    ratings.clear();
    std::stringstream ss(ratingsStr);
    std::string ratingPiece;
    while (std::getline(ss, ratingPiece, ',')) {
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

// Getters
const Address& User::getAddress() const { return address; }
std::string User::getContactInfo() const { return contactInfo; }
const std::vector<int>& User::getRatings() const { return ratings; }
const std::vector<std::string>& User::getReviews() const { return reviews; }

// Setters
void User::setAddress(const Address& address) { this->address = address; }
void User::setContactInfo(const std::string& contactInfo) { this->contactInfo = contactInfo; }