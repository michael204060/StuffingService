#include "User.h"
#include <iomanip>

void User::input() {
    cout << "Enter first name: ";
    getline(cin, firstName);
    cout << "Enter last name: ";
    getline(cin, lastName);
    cout << "Enter password: ";
    getline(cin, password);
    address.input();
    cout << "Enter contact info: ";
    getline(cin, contactInfo);
}

void User::display() const {
    cout << "User Profile: " << firstName << " " << lastName << endl;
    address.display();
    cout << "Contact info: " << contactInfo << endl;
    cout << "Average rating: " << fixed << setprecision(1) << getAverageRating() << endl;
}

void User::addRating(int rating) { ratings.push_back(rating); }
void User::addReview(const string& review) { reviews.push_back(review); }

double User::getAverageRating() const {
    if (ratings.empty()) return 0;
    double sum = 0;
    for (int r : ratings) sum += r;
    return sum / ratings.size();
}

void User::save(ofstream& outFile) const {
    outFile.write("U", 1);
    Person::save(outFile);
    address.save(outFile);

    size_t size = contactInfo.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(contactInfo.c_str(), size);

    size = ratings.size(); outFile.write((char*)&size, sizeof(size));
    for (int r : ratings) outFile.write((char*)&r, sizeof(r));

    size = reviews.size(); outFile.write((char*)&size, sizeof(size));
    for (const string& review : reviews) {
        size = review.size(); outFile.write((char*)&size, sizeof(size)); outFile.write(review.c_str(), size);
    }
}

void User::load(ifstream& inFile) {
    Person::load(inFile);
    address.load(inFile);

    size_t size;
    char* buffer;

    inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; contactInfo = buffer; delete[] buffer;

    inFile.read((char*)&size, sizeof(size));
    ratings.resize(size);
    for (size_t i = 0; i < size; ++i) inFile.read((char*)&ratings[i], sizeof(ratings[i]));

    inFile.read((char*)&size, sizeof(size));
    reviews.resize(size);
    for (size_t i = 0; i < size; ++i) {
        inFile.read((char*)&size, sizeof(size)); buffer = new char[size + 1]; inFile.read(buffer, size); buffer[size] = '\0'; reviews[i] = buffer; delete[] buffer;
    }
}
