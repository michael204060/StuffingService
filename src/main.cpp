#include "headers/Address.h"
#include "headers/Admin.h"
#include "headers/Person.h"
#include "headers/User.h"
#include "headers/Specialist.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>
#include <fstream>

using namespace std;

void save(const vector<Person*>& people) {
    ofstream file("people.bin", ios::binary | ios::trunc);
    if (!file.is_open()) {
        cout << "Error: Could not open people.bin for writing." << endl;
        return;
    }
    for (const Person* person : people) {
        int userType = 0;
        if (dynamic_cast<const User*>(person)) {
            userType = 1;
        } else if (dynamic_cast<const Specialist*>(person)) {
            userType = 2;
        } else if (dynamic_cast<const Admin*>(person)) {
            userType = 3;
        }

        file.write(reinterpret_cast<const char*>(&userType), sizeof(userType));
        person->save(file);
    }
    file.close();
}

void load(vector<Person*>& people) {
    ifstream file("people.bin", ios::binary);
    if (!file.is_open()) {
        cout << "people.bin not found. Would you like to create a new file? (yes/no): ";
        string response;
        getline(cin, response);
        if (response == "yes") {
            save(people);
            cout << "New file created." << endl;
        }
        return;
    }

    while (true) {
        int userType;
        if (!file.read(reinterpret_cast<char*>(&userType), sizeof(userType))) {
            if (file.eof()) break;
            cout << "Error: Failed to read userType." << endl;
            break;
        }

        Person* person = nullptr;
        switch (userType) {
            case 1:
                person = new (nothrow) User();
                break;
            case 2:
                person = new (nothrow) Specialist();
                break;
            case 3:
                person = new (nothrow) Admin();
                break;
            default:
                cout << "Error: Invalid userType." << endl;
                continue;
        }

        if (person == nullptr) {
            cout << "Memory allocation error: Failed to allocate memory for person object." << endl;
            break;
        }

        try {
            person->load(file);
            people.push_back(person);
        } catch (const std::bad_alloc& e) {
           // cout << "Memory allocation error: " << e.what() << endl;
           // delete person;
            break;
        } catch (const std::exception& e) {
            cout << "Error loading person data: " << e.what() << endl;
            delete person;
            break;
        }
    }

    file.close();
}

void deleteAccountByName(vector<Person*>& people) {
    string firstName, lastName;
    cout << "Enter first name: ";
    getline(cin, firstName);
    cout << "Enter last name: ";
    getline(cin, lastName);

    auto it = find_if(people.begin(), people.end(), [&](const Person* person) {
        return person->getFirstName() == firstName && person->getLastName() == lastName;
    });

    if (it != people.end()) {
        delete *it;
        people.erase(it);
        cout << "Account deleted successfully." << endl;
    } else {
        cout << "Person not found." << endl;
    }
}

int main() {
    vector<Person*> people;
    load(people);
    int choice;

    do {
        cout << "Main Menu:" << endl;
        cout << "1. Create new user" << endl;
        cout << "2. Display all people" << endl;
        cout << "3. View profile information by name" << endl;
        cout << "4. Log in" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                int userType;
                cout << "Select user type: 1. User, 2. Specialist, 3. Admin: ";
                cin >> userType;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                Person* person = nullptr;
                if (userType == 1) {
                    person = new (nothrow) User();
                } else if (userType == 2) {
                    person = new (nothrow) Specialist();
                } else if (userType == 3) {
                    person = new (nothrow) Admin();
                } else {
                    cout << "Invalid user type selected." << endl;
                    break;
                }

                if (person == nullptr) {
                    cout << "Error: Memory allocation failed while creating new user." << endl;
                    break;
                }

                person->input();
                people.push_back(person);
                cout << "User created successfully." << endl;
                break;
            }
            case 2: {
                for (const Person* person : people) {
                    person->display();
                    cout << "--------------------------" << endl;
                }
                break;
            }
            case 3: {
                string firstName, lastName;
                cout << "Enter first name: ";
                getline(cin, firstName);
                cout << "Enter last name: ";
                getline(cin, lastName);

                auto it = find_if(people.begin(), people.end(), [&](const Person* person) {
                    return person->getFirstName() == firstName && person->getLastName() == lastName;
                });

                if (it != people.end()) {
                    (*it)->display();
                } else {
                    cout << "Person not found." << endl;
                }
                break;
            }
            case 4: {
                string firstName, lastName, password;
                cout << "Enter first name: ";
                getline(cin, firstName);
                cout << "Enter last name: ";
                getline(cin, lastName);
                cout << "Enter password: ";
                getline(cin, password);

                auto it = find_if(people.begin(), people.end(), [&](const Person* person) {
                    return person->getFirstName() == firstName && person->getLastName() == lastName && person->checkPassword(password);
                });

                if (it != people.end()) {
                    cout << "Login successful." << endl;

                    int loginChoice;
                    do {
                        cout << "1. View Profile" << endl;
                        cout << "2. Delete Account" << endl;
                        cout << "3. Edit Profile" << endl;

                        User* user = dynamic_cast<User*>(*it);
                        Specialist* specialist = dynamic_cast<Specialist*>(*it);
                        Admin* admin = dynamic_cast<Admin*>(*it);

                        if (user) {
                            cout << "4. Rate and review a Specialist" << endl;
                        } else if (specialist) {
                            cout << "4. Rate and review a User" << endl;
                        } else if (admin) {
                            cout << "4. Delete User by name" << endl;
                        }

                        cout << "0. Logout" << endl;
                        cout << "Enter your choice: ";
                        cin >> loginChoice;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        switch (loginChoice) {
                            case 1:
                                (*it)->display();
                                break;
                            case 2: {
                                string confirmPassword;
                                cout << "Are you sure you want to delete the account? (yes/no): ";
                                getline(cin, confirmPassword);
                                if (confirmPassword == "yes") {
                                    cout << "Enter password to confirm: ";
                                    getline(cin, confirmPassword);
                                    if ((*it)->checkPassword(confirmPassword)) {
                                        delete *it;
                                        people.erase(it);
                                        cout << "Account deleted successfully." << endl;
                                        loginChoice = 0;
                                    } else {
                                        cout << "Incorrect password." << endl;
                                    }
                                }
                                break;
                            }
                            case 3:
                                (*it)->input();
                                break;
                            case 4:
                                deleteAccountByName(people);
                                break;
                            case 0:
                                cout << "Logged out." << endl;
                                break;
                            default:
                                cout << "Invalid choice. Try again." << endl;
                        }
                    } while (loginChoice != 0);
                } else {
                    cout << "Invalid login credentials." << endl;
                }
                break;
            }
            case 0:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 0);

    save(people);

    for (Person* person : people) {
        delete person;
    }
    people.clear();
    return 0;
}
