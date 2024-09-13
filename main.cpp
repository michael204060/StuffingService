#include "interaction.h"

#include <iostream>   
#include <limits>     
#include <ios>        
#include <algorithm>

int main() {
    vector<Person*> people;
    
    loadAll(people);

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
                    person = new User();
                } else if (userType == 2) {
                    person = new Specialist();
                } else if (userType == 3) {
                    person = new Admin();
                }

                if (person) {
                    person->input();
                    people.push_back(person);
                    cout << "User created successfully." << endl;
                } else {
                    cout << "Invalid user type selected." << endl;
                }
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
                            cout << "4. Manage User or Specialist Account" << endl;
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
                            case 4: {
                                if (user) {
                                    
                                    string specFirstName, specLastName;
                                    cout << "Enter the first name of the Specialist: ";
                                    getline(cin, specFirstName);
                                    cout << "Enter the last name of the Specialist: ";
                                    getline(cin, specLastName);

                                    auto specIt = find_if(people.begin(), people.end(), [&](const Person* person) {
                                        Specialist* spec = dynamic_cast<Specialist*>(const_cast<Person*>(person));
                                        return spec && spec->getFirstName() == specFirstName && spec->getLastName() == specLastName;
                                    });

                                    if (specIt != people.end()) {
                                        int rating;
                                        do {
                                            cout << "Enter rating (0 to 10): ";
                                            cin >> rating;
                                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        } while (rating < 0 || rating > 10);

                                        string review;
                                        cout << "Enter review: ";
                                        getline(cin, review);

                                        Specialist* spec = dynamic_cast<Specialist*>(*specIt);
                                        spec->addRating(rating);
                                        spec->addReview(review);
                                        cout << "Rating and review added successfully." << endl;
                                    } else {
                                        cout << "Specialist not found." << endl;
                                    }
                                } else if (specialist) {
                                    
                                    string userFirstName, userLastName;
                                    cout << "Enter the first name of the User: ";
                                    getline(cin, userFirstName);
                                    cout << "Enter the last name of the User: ";
                                    getline(cin, userLastName);

                                    auto userIt = find_if(people.begin(), people.end(), [&](const Person* person) {
                                        User* u = dynamic_cast<User*>(const_cast<Person*>(person));
                                        return u && u->getFirstName() == userFirstName && u->getLastName() == userLastName;
                                    });

                                    if (userIt != people.end()) {
                                        int rating;
                                        do {
                                            cout << "Enter rating (0 to 10): ";
                                            cin >> rating;
                                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        } while (rating < 0 || rating > 10);

                                        string review;
                                        cout << "Enter review: ";
                                        getline(cin, review);

                                        User* usr = dynamic_cast<User*>(*userIt);
                                        usr->addRating(rating);
                                        usr->addReview(review);
                                        cout << "Rating and review added successfully." << endl;
                                    } else {
                                        cout << "User not found." << endl;
                                    }
                                } else if (admin) {
                                    
                                    string personFirstName, personLastName;
                                    cout << "Enter the first name of the person to manage: ";
                                    getline(cin, personFirstName);
                                    cout << "Enter the last name of the person to manage: ";
                                    getline(cin, personLastName);

                                    auto personIt = find_if(people.begin(), people.end(), [&](const Person* person) {
                                        return person->getFirstName() == personFirstName && person->getLastName() == personLastName && !dynamic_cast<Admin*>(const_cast<Person*>(person));
                                    });

                                    if (personIt != people.end()) {
                                        cout << "Account found. Deleting account." << endl;
                                        delete *personIt;
                                        people.erase(personIt);
                                        cout << "Account deleted successfully." << endl;
                                    } else {
                                        cout << "Person not found or is an admin." << endl;
                                    }
                                }
                                break;
                            }
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

    
    saveAll(people);

    
    for (Person* person : people) {
        delete person;
    }

    return 0;
}