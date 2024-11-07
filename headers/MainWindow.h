//
// Created by micha on 31.10.2024.
//

#ifndef AWESOME_PROJECT_PLUS_WINDOW_H
#define AWESOME_PROJECT_PLUS_WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QListWidget>
#include <vector>
#include <QString>
#include <functional>
#include <QPixmap>  // Добавляем для QPixmap
#include <QPainter> // Добавляем для QPainter
#include "../headers/Person.h"
#include "../headers/Database.h"
#include "../headers/Admin.h"
#include "../headers/Specialist.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(Database& db, QWidget* parent = nullptr);
    ~MainWindow();

    private slots:
        void addNewClient();
    void createUser();
    void createSpecialist();
    void createAdmin();
    void displayAllClients();
    void displayClientDetails(QListWidgetItem* item);
    void findInfoByClientName();
    void login();
    void handleLogin(Person* person);
    void editProfile(Person* person);
    void deleteAccount(Person* person);
    void rateAndReviewUser(Specialist* specialist);
    void rateAndReviewSpecialist(User* user);
    void deleteUserByName(Admin* admin);
    void logout();

private:
    Database& db;
    QLabel* outputLabel;
    QVBoxLayout* mainLayout;
    std::vector<Person*> people;

    // Добавляем переменную для хранения фонового изображения
    QPixmap backgroundImage; // Или QImage, если вы предпочитаете

    void mainMenu();
    void createMainMenuButtons();
    void clearLayout(QLayout* layout);
    void loadFromDatabase();
    void saveToDatabase();

    QLineEdit* createLineEdit(const QString& placeholder = "");
    QPushButton* createButton(const QString& text, std::function<void()> callback);
    void createPersonInputForm(const QString& type);

protected:
    // Переопределяем метод для рисования
    void paintEvent(QPaintEvent* event) override;
};

#endif // AWESOME_PROJECT_PLUS_WINDOW_H