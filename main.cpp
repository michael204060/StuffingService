#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <iomanip>

using namespace std;

// Определение структуры адреса
struct Address {
    string country;
    string region;
    string city;
    string street;
    string house;
    string apartment;

    void input() {
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

    void display() const {
        cout << "Address: " << country << ", " << region << ", " << city;
        if (!street.empty()) cout << ", " << street;
        if (!house.empty()) cout << ", House " << house;
        if (!apartment.empty()) cout << ", Apt " << apartment;
        cout << endl;
    }
};

// Базовый класс пользователя
class Person {
protected:
    string firstName;
    string lastName;
    string password;

public:
    virtual void input() = 0;
    virtual void display() const = 0;
    virtual ~Person() {}

    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    bool checkPassword(const string& pass) const { return pass == password; }
};

// Класс пользователя
class User : public Person {
private:
    Address address;
    string contactInfo;
    vector<int> ratings;
    vector<string> reviews;

public:
    void input() override {
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

    void display() const override {
        cout << "User Profile: " << firstName << " " << lastName << endl;
        address.display();
        cout << "Contact info: " << contactInfo << endl;
        cout << "Average rating: " << fixed << setprecision(1) << getAverageRating() << endl;
    }

    void addRating(int rating) {
        ratings.push_back(rating);
    }

    void addReview(const string& review) {
        reviews.push_back(review);
    }

    double getAverageRating() const {
        if (ratings.empty()) return 0;
        double sum = 0;
        for (int r : ratings) sum += r;
        return sum / ratings.size();
    }
};

// Класс специалиста
class Specialist : public Person {
private:
    string jobProfile;
    Address address;
    string contactInfo;
    vector<int> ratings;
    vector<string> reviews;

public:
    void input() override {
        cout << "Enter first name: ";
        getline(cin, firstName);
        cout << "Enter last name: ";
        getline(cin, lastName);
        cout << "Enter password: ";
        getline(cin, password);
        cout << "Enter job profile: ";
        getline(cin, jobProfile);
        address.input();
        cout << "Enter contact info: ";
        getline(cin, contactInfo);
    }

    void display() const override {
        cout << "Specialist Profile: " << firstName << " " << lastName << endl;
        cout << "Job Profile: " << jobProfile << endl;
        address.display();
        cout << "Contact info: " << contactInfo << endl;
        cout << "Average rating: " << fixed << setprecision(1) << getAverageRating() << endl;
    }

    void addRating(int rating) {
        ratings.push_back(rating);
    }

    void addReview(const string& review) {
        reviews.push_back(review);
    }

    double getAverageRating() const {
        if (ratings.empty()) return 0;
        double sum = 0;
        for (int r : ratings) sum += r;
        return sum / ratings.size();
    }
};

// Класс администратора
class Admin : public Person {
public:
    void input() override {
        cout << "Enter first name: ";
        getline(cin, firstName);
        cout << "Enter last name: ";
        getline(cin, lastName);
        cout << "Enter password: ";
        getline(cin, password);
    }

    void display() const override {
        cout << "Admin Profile: " << firstName << " " << lastName << endl;
    }
};

// Функция для проверки числового ввода
int getValidatedInt(const string& prompt, int minValue = 0, int maxValue = 10) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (!cin.fail() && value >= minValue && value <= maxValue) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
            return value;
        }
        cout << "Invalid input, please enter a number between " << minValue << " and " << maxValue << "." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
    }
}

// Функция для поиска пользователя по имени и фамилии
Person* findPerson(vector<Person*>& people, const string& firstName, const string& lastName) {
    for (Person* person : people) {
        if (person->getFirstName() == firstName && person->getLastName() == lastName) {
            return person;
        }
    }
    return nullptr;
}

// Функция для меню авторизации
void accountMenu(Person* person, vector<Person*>& people) {
    int choice;
    do {
        cout << "\nAccount Menu:\n";
        cout << "1. View profile information\n";
        cout << "2. Delete account\n";
        cout << "3. Edit profile\n";
        cout << "0. Log out\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                person->display();
                break;
            case 2: {
                string confirmPassword;
                cout << "Are you sure you want to delete your account? (Enter your password to confirm): ";
                getline(cin, confirmPassword);
                if (person->checkPassword(confirmPassword)) {
                    auto it = find(people.begin(), people.end(), person);
                    if (it != people.end()) {
                        delete *it;
                        people.erase(it);
                        cout << "Account deleted successfully." << endl;
                        return;
                    }
                } else {
                    cout << "Incorrect password." << endl;
                }
                break;
            }
            case 3:
                cout << "Re-enter your profile information." << endl;
                person->input();
                break;
            case 0:
                cout << "Logging out..." << endl;
                break;
            default:
                cout << "Invalid choice, please try again." << endl;
        }
    } while (choice != 0);
}

// Основное меню
void mainMenu(vector<Person*>& people);

int main() {
    vector<Person*> people;
    mainMenu(people);

    for (Person* person : people) {
        delete person;
    }

    return 0;
}

void mainMenu(vector<Person*>& people) {
    int choice;
    do {
        cout << "\nMain Menu:\n";
        cout << "1. Create new user\n";
        cout << "2. Display all people\n";
        cout << "3. Search profile by name\n";
        cout << "4. Log in to your account\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера ввода

        switch (choice) {
            case 1: {
                cout << "Choose type of person to create:\n";
                cout << "1. User\n";
                cout << "2. Specialist\n";
                cout << "3. Admin\n";
                int personType;
                cin >> personType;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                Person* person = nullptr;
                switch (personType) {
                    case 1:
                        person = new User();
                        break;
                    case 2:
                        person = new Specialist();
                        break;
                    case 3:
                        person = new Admin();
                        break;
                    default:
                        cout << "Invalid type selected." << endl;
                        continue;
                }

                person->input();
                people.push_back(person);
                cout << "New person created successfully." << endl;
                break;
            }
            case 2: {
                for (Person* person : people) {
                    person->display();
                    cout << "-----------------------" << endl;
                }
                break;
            }
            case 3: {
                string firstName, lastName;
                cout << "Enter first name: ";
                getline(cin, firstName);
                cout << "Enter last name: ";
                getline(cin, lastName);
                Person* person = findPerson(people, firstName, lastName);
                if (person) {
                    person->display();
                } else {
                    cout << "Person not found." << endl;
                }
                break;
            }
            case 4: {
                string firstName, lastName, password;
                cout << "Enter your first name: ";
                getline(cin, firstName);
                cout << "Enter your last name: ";
                getline(cin, lastName);
                cout << "Enter your password: ";
                getline(cin, password);
                Person* person = findPerson(people, firstName, lastName);
                if (person && person->checkPassword(password)) {
                    accountMenu(person, people);
                } else {
                    cout << "Invalid login credentials." << endl;
                }
                break;
            }
            case 0:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice, please try again." << endl;
        }
    } while (choice != 0);
}
