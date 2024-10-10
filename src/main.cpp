#include "headers/Address.h"
#include "headers/Admin.h"
#include "headers/Person.h"
#include "headers/User.h"
#include "headers/Specialist.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>
#include <sqlite3.h>
#include <string>
#include <windows.h>
#include <sstream>
sqlite3 *db;
void initializeDatabase()
{
    int rc = sqlite3_open("people.db", &db);
    if (rc)
    {
        std::cerr << "Error of database opining: " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    }
    std::string sql;
    sql = "create table if not exists people ("
          "id integer primary key autoincrement,"
          "type TEXT,"
          "firstName TEXT,"
          "lastName TEXT,"
          "password TEXT,"
          "country TEXT,"
          "region TEXT,"
          "city TEXT,"
          "street TEXT,"
          "house TEXT,"
          "apartment TEXT,"
          "contactInfo TEXT,"
          "specialization TEXT,"
          "certifications TEXT,"
          "privileges TEXT,"
          "ratings TEXT,"
          "reviews TEXT"
          ");";
    char *errmsg;
    rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errmsg << std::endl;
        sqlite3_free(errmsg);
        exit(1);
    }
}
void saveToDatabase(const std::vector<Person *> &people)
{sqlite3_exec(db, "DELETE FROM people;", nullptr, nullptr, nullptr);

    std::string sql = "INSERT INTO people (type, firstName, lastName, password, country, region, city, street, house, apartment, contactInfo, specialization, certifications, privileges, ratings, reviews) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error (prepare): " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    }

    for (const Person *person : people) {
        if (person == nullptr) {
            std::cerr << "Error: Null pointer to person object." << std::endl;
            continue;
        }

        std::string type;
        if (dynamic_cast<const User *>(person)) {
            type = "User";
        } else if (dynamic_cast<const Specialist *>(person)) {
            type = "Specialist";
        } else if (dynamic_cast<const Admin *>(person)) {
            type = "Admin";
        } else {
            std::cerr << "Unknown person type." << std::endl;
            continue;
        }

        int index = 1;
        sqlite3_bind_text(stmt, index++, type.c_str(), -1, SQLITE_TRANSIENT);

        person->bindToStatement(stmt, index);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "SQL error (insert): " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            exit(1);
        }

        rc = sqlite3_reset(stmt);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error (reset): " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            exit(1);
        }
    }

    sqlite3_finalize(stmt);

}
void loadFromDatabase(std::vector<Person *> &people)
{
    for (Person *person : people)
    {
        delete person;
    }
    people.clear();
    std::string sql = "select * from people;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
     if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    }
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string type = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        Person *person = nullptr;
        if (type == "User")
        {
            person = new User();
        }
        else if (type == "Specialist")
        {
            person = new Specialist();
        }
        else if (type == "Admin")
        {
            person = new Admin();
        }
        if (person)
        {
            person->loadFromStatement(stmt);
            people.push_back(person);
        }
    }
    sqlite3_finalize(stmt);
}
void deleteAccountByName(std::vector<Person *> &people)
{
    std::string firstName, lastName;
    std::cout << "Please input your first name: ";
    getline(std::cin, firstName);
    std::cout << "Please input your second name: ";
    getline(std::cin, lastName);
    auto it = find_if(people.begin(), people.end(), [&](const Person *person) {
        return person->getFirstName() == firstName && person->getLastName() == lastName;
    });
    if (it != people.end())
    {
        delete *it;
        people.erase(it);
       std::cout << "Account is successfully deleted." << std::endl;
    }
    else
    {
        std::cout << "User is not found." << std::endl;
    }
}
int main() {
    std::vector<Person*> people;
    initializeDatabase();
    loadFromDatabase(people);
    int choice;
    do {
        std::cout << "\nMain menu:" << std::endl;
        std::cout << "1.Add new client" << std::endl;
        std::cout << "2.Display all clients" << std::endl;
        std::cout << "3.Find info by client's name" << std::endl;
        std::cout << "4.Log in" << std::endl;
        std::cout << "0.Exit" << std::endl;
        std::cout << "Enter your choice: ";
        while (!(std::cin >> choice)) {
            std::cout << "Incorrect input. Please enter the number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 1: {
                int userType;
                std::cout << "Please choose the type of client (1 - user, 2 - specialist, 3 - admin): ";
                while (!(std::cin >> userType)) {
                    std::cout << "Incorrect input. Please enter the number: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                Person* person = nullptr;
                if (userType == 1) {
                    person = new User();  
                } else if (userType == 2) {
                    person = new Specialist();  
                } else if (userType == 3) {
                    person = new Admin();  
                } else {
                    std::cout << "Incorrect client type." << std::endl;
                    break;
                }
                if (person == nullptr) {
                    std::cout << "Memory error." << std::endl;
                    break;
                }
                person->input();
                people.push_back(person);
                saveToDatabase(people);
                std::cout << "Client successfully added." << std::endl;
                break;
            }
            case 2: {
                loadFromDatabase(people); 
                 if(people.empty()) {
                    std::cout << "Clients list is empty.\n";
                } else {
                    for (const Person* person : people) {
                         person->display();
                         std::cout << "--------------------------" << std::endl;
                     }
                }
                break;
            }
            case 3: {
                std::string firstName, lastName;
                std::cout << "Please input first name: ";
                std::getline(std::cin, firstName);
                std::cout << "Please input last name: ";
                getline(std::cin, lastName);
                loadFromDatabase(people); 
                auto it = find_if(people.begin(), people.end(), [&](const Person* person) {
                    return person->getFirstName() == firstName && person->getLastName() == lastName;
                });
                if (it != people.end()) {
                    (*it)->display();
                } else {
                    std::cout << "Client isn't found." << std::endl;
                }
                break;
            }
            case 4: {
                std::string firstName, lastName, password;
                std::cout << "Please input first name: ";
                getline(std::cin, firstName);
                std::cout << "Please input last name: ";
                getline(std::cin, lastName);
                std::cout << "Please input password: ";
                getline(std::cin, password);
                loadFromDatabase(people); 
                auto it = find_if(people.begin(), people.end(), [&](const Person* person) {
                    return person->getFirstName() == firstName && person->getLastName() == lastName && person->checkPassword(password);
                });
                if (it != people.end()) {
                    std::cout << "Log in successed." << std::endl;
                    int loginChoice;
                    do {
                        std::cout << "1.See profile"<< std::endl;
                        std::cout << "2.Delete account" << std::endl;
                        std::cout << "3.Edit profile" << std::endl;
                        User* user = dynamic_cast<User*>(*it);
                        Specialist* specialist = dynamic_cast<Specialist*>(*it);
                        Admin* admin = dynamic_cast<Admin*>(*it);
                        if (user) {
                            std::cout << "4.Rate and give review of specialist" << std::endl;
                        } else if (specialist) {
                            std::cout << "4.Rate and give review of user" << std::endl;
                        } else if (admin) {
                            std::cout << "4.Delete user by name and second name"<<std::endl;
                        }
                        std::cout << "0.Exit to the main menu" << std::endl;
                        std::cout << "Enter your choice: ";
                        while (!(std::cin >> loginChoice)) {
                            std::cout << "Incorrect input. Please enter the number: ";
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        switch (loginChoice) {
                            case 1:
                                (*it)->display();
                                break;
                            case 2: {
                                std::string confirmPassword;
                                std::cout << "Do you confirm the deletion of account? (yes/no): ";
                                getline(std::cin, confirmPassword);
                                if (confirmPassword == "yes") {
                                    std::cout << "Please input the password t0 confirm: ";
                                    getline(std::cin, confirmPassword);
                                    if ((*it)->checkPassword(confirmPassword)) {
                                        delete *it;
                                        people.erase(it);
                                        saveToDatabase(people);
                                        std::cout << "Account is deleted successfully." << std::endl;
                                        loginChoice = 0;
                                    } else {
                                        std::cout << "Invalid password." << std::endl;
                                    }
                                }
                                break;
                            }
                            case 3:
                                (*it)->input();
                                saveToDatabase(people);
                                std::cout << "Profile is changed successfully." << std::endl;
                                break;
                            case 4: {
                                if (user) {
                                    std::string specialistFirstName, specialistLastName;
                                    std::cout << "Please input the first name of the specialist you wanna rate: ";
                                    getline(std::cin, specialistFirstName);
                                    std::cout << "Please input the second name of the specialist you wanna rate: ";
                                    getline(std::cin, specialistLastName);
                                    auto specialistIt = find_if(people.begin(), people.end(), [&](const Person* p) {
                                        return p->getFirstName() == specialistFirstName && p->getLastName() == specialistLastName;
                                    });
                                    if (specialistIt != people.end()) {
                                        Specialist* specialist = dynamic_cast<Specialist*>(*specialistIt);
                                        if (specialist) {
                                            int rating;
                                            std::cout << "Input raiting (1-5): ";
                                            while (!(std::cin >> rating) || rating < 1 || rating > 5) {
                                                std::cout << "Incorrect input, please enter a number from 1 to 5: ";
                                                std::cin.clear();
                                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                            }
                                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                            std::string review;
                                            std::cout << "Please get your review: ";
                                            std::getline(std::cin, review);
                                            specialist->addRating(rating);
                                            specialist->addReview(review);
                                            saveToDatabase(people);
                                            std::cout << "Raiting and review were added successfully." << std::endl;
                                        } else {
                                            std::cout << "This guy isn't specialist." << std::endl;
                                        }
                                    } else {
                                        std::cout << "Specialist isn't found." << std::endl;
                                    }
                                } else if (specialist) {
                                    std::string userFirstName, userLastName;
                                    std::cout << "Please input the first name of the user you wanna rate: "; 
                                    getline(std::cin, userFirstName);
                                    std::cout << "Please input the second name of the user you wanna rate: "; 
                                    getline(std::cin, userLastName);
                                    auto userIt = find_if(people.begin(), people.end(), [&](const Person* p) {
                                        return p->getFirstName() == userFirstName && p->getLastName() == userLastName;
                                    });
                                    if (userIt != people.end()) {
                                        User* user = dynamic_cast<User*>(*userIt);
                                        if (user) {
                                            int rating;
                                            std::cout << "Input raiting (1-5): ";
                                            while (!(std::cin >> rating) || rating < 1 || rating > 5) {
                                                std::cout << "Incorrect input, please enter a number from 1 to 5: ";
                                                std::cin.clear();
                                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                            }
                                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                            std::string review;
                                            std::cout << "Please get your review: ";
                                            std::getline(std::cin, review);
                                            user->addRating(rating);
                                            user->addReview(review);
                                            saveToDatabase(people);
                                            std::cout << "Raiting and review were added successfully." << std::endl;
                                        } else {
                                            std::cout << "This guy isn't user." << std::endl;
                                        }
                                    } else {
                                        std::cout << "User isn't found." << std::endl;
                                    }
                                } else if (admin) {
                                    std::string personFirstName, personLastName;
                                    std::cout << "Please enter the first name of the client you wanna delete: ";
                                    getline(std::cin, personFirstName);
                                    std::cout << "Please enter the second name of the client you wanna delete: ";
                                    getline(std::cin, personLastName);
                                    auto personIt = find_if(people.begin(), people.end(), [&](const Person* p) {
                                        return p->getFirstName() == personFirstName && p->getLastName() == personLastName;
                                    });
                                    if (personIt != people.end()) {
                                        delete *personIt;
                                        people.erase(personIt);
                                        saveToDatabase(people);
                                        std::cout << "Client is deleted successfully." << std::endl;
                                    } else {
                                        std::cout << "Client isn't found." << std::endl;
                                    }
                                }
                                break;
                            }
                            case 0:
                                std::cout << "Exit to the main meny." << std::endl;
                                break;
                            default:
                                std::cout << "Incorrect choice, please try again." << std::endl;
                        }
                    } while (loginChoice != 0);
                }
                else
                {
                    std::cout << "Invalid credentails for log in." << std::endl;
                }
                break;
            }
            case 0: {
                std::cout << "Exit." << std::endl;
                break;
            }
            default: {
                std::cout << "Incorrect choice, please try again." << std::endl;
            }
        }
    } while (choice != 0);
    for (Person *p : people)
    {
        delete p;
    }
    people.clear();
    sqlite3_close(db);
    return 0;
}
