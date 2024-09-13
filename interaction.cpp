#include "interaction.h"

// Методы Address
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

// Методы Person
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

// Методы User
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
    outFile.write("U", 1);  // Маркер типа User
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

// Методы Specialist (аналогичны User)
void Specialist::input() {
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

void Specialist::display() const {
    cout << "Specialist Profile: " << firstName << " " << lastName << endl;
    address.display();
    cout << "Contact info: " << contactInfo << endl;
    cout << "Average rating: " << fixed << setprecision(1) << getAverageRating() << endl;
}

void Specialist::addRating(int rating) { ratings.push_back(rating); }
void Specialist::addReview(const string& review) { reviews.push_back(review); }

double Specialist::getAverageRating() const {
    if (ratings.empty()) return 0;
    double sum = 0;
    for (int r : ratings) sum += r;
    return sum / ratings.size();
}

void Specialist::save(ofstream& outFile) const {
    outFile.write("S", 1);  // Маркер типа Specialist
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

void Specialist::load(ifstream& inFile) {
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

// Методы Admin (аналогичны User)
void Admin::input() {
    cout << "Enter first name: ";
    getline(cin, firstName);
    cout << "Enter last name: ";
    getline(cin, lastName);
    cout << "Enter password: ";
    getline(cin, password);
}

void Admin::display() const {
    cout << "Admin Profile: " << firstName << " " << lastName << endl;
}

void Admin::save(ofstream& outFile) const {
    outFile.write("A", 1);  // Маркер типа Admin
    Person::save(outFile);
}

void Admin::load(ifstream& inFile) {
    Person::load(inFile);
}

// Функции загрузки и сохранения всех пользователей
void saveAll(const vector<Person*>& people) {
    ofstream outFile("people.dat", ios::binary);
    size_t size = people.size();
    outFile.write((char*)&size, sizeof(size));
    for (const Person* person : people) {
        person->save(outFile);
    }
}

void loadAll(vector<Person*>& people) {
    ifstream inFile("people.dat", ios::binary);
    if (!inFile) return;

    size_t size;
    inFile.read((char*)&size, sizeof(size));
    people.resize(size);

    for (size_t i = 0; i < size; ++i) {
        char type;
        inFile.read(&type, sizeof(type));

        Person* person = nullptr;
        if (type == 'U') person = new User();
        else if (type == 'S') person = new Specialist();
        else if (type == 'A') person = new Admin();

        if (person) {
            person->load(inFile);
            people[i] = person;
        }
    }
}
