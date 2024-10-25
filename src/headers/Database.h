#ifndef AWESOMEPROJECTPLUS_DATABASE_H
#define AWESOMEPROJECTPLUS_DATABASE_H

#include <vector>
#include <sqlite3.h>
#include "headers/Person.h"

class Database {
private:
    sqlite3* db;

public:
    Database();
    ~Database();

    void initializeDatabase();
    void saveToDatabase(const std::vector<Person*>& people);
    std::vector<Person*> loadFromDatabase();
};

#endif 