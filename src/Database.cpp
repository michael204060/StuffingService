#include "../headers/Database.h"
#include <iostream>
#include <string>
#include "../headers/Address.h"
#include "../headers/User.h"
#include "../headers/Admin.h"
#include "../headers/Specialist.h"

Database::Database() : db(nullptr) {}

Database::~Database() {
    if (db != nullptr) {
        sqlite3_close(db);
    }
}

void Database::initializeDatabase() {
    int rc = sqlite3_open("people.db", &db);
    if (rc) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    }
    std::string sql;
    sql = "CREATE TABLE IF NOT EXISTS people ("
          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
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
    char* errmsg;
    rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errmsg << std::endl;
        sqlite3_free(errmsg);
        exit(1);
    }
}

void Database::saveToDatabase(const std::vector<Person*>& people) {
    sqlite3_exec(db, "DELETE FROM people;", nullptr, nullptr, nullptr);

    std::string sql = "INSERT INTO people (type, firstName, lastName, password, country, region, city, street, house, apartment, contactInfo, specialization, certifications, privileges, ratings, reviews) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error (prepare): " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    }

    for (const Person* person : people) {
        if (person == nullptr) {
            std::cerr << "Error: Null pointer to person object." << std::endl;
            continue;
        }

        std::string type;
        if (dynamic_cast<const User*>(person)) {
            type = "User";
        }
        else if (dynamic_cast<const Specialist*>(person)) {
            type = "Specialist";
        }
        else if (dynamic_cast<const Admin*>(person)) {
            type = "Admin";
        }
        else {
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

std::vector<Person*> Database::loadFromDatabase() {
    std::vector<Person*> people;
    std::string sql = "SELECT * FROM people;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        Person* person = nullptr;
        if (type == "User") {
            person = new User();
        }
        else if (type == "Specialist") {
            person = new Specialist();
        }
        else if (type == "Admin") {
            person = new Admin();
        }
        if (person) {
            person->loadFromStatement(stmt);
            people.push_back(person);
        }
    }
    sqlite3_finalize(stmt);
    return people;
}