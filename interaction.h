#ifndef INTERACTION_H
#define INTERACTION_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
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

    void input();
    void display() const;
    void save(ofstream& outFile) const;
    void load(ifstream& inFile);
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

    virtual void save(ofstream& outFile) const;
    virtual void load(ifstream& inFile);

    string getFirstName() const;
    string getLastName() const;
    bool checkPassword(const string& pass) const;
};

// Класс пользователя
class User : public Person {
private:
    Address address;
    string contactInfo;
    vector<int> ratings;
    vector<string> reviews;

public:
    void input() override;
    void display() const override;

    void addRating(int rating);
    void addReview(const string& review);
    double getAverageRating() const;

    void save(ofstream& outFile) const override;
    void load(ifstream& inFile) override;
};

// Класс специалиста
class Specialist : public Person {
private:
    Address address;
    string contactInfo;
    vector<int> ratings;
    vector<string> reviews;

public:
    void input() override;
    void display() const override;

    void addRating(int rating);
    void addReview(const string& review);
    double getAverageRating() const;

    void save(ofstream& outFile) const override;
    void load(ifstream& inFile) override;
};

// Класс администратора
class Admin : public Person {
public:
    void input() override;
    void display() const override;

    void save(ofstream& outFile) const override;
    void load(ifstream& inFile) override;
};

// Функции загрузки и сохранения всех пользователей
void saveAll(const vector<Person*>& people);
void loadAll(vector<Person*>& people);

#endif // INTERACTION_H
