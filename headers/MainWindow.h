#ifndef AWESOME_PROJECT_PLUS_WINDOW_H
#define AWESOME_PROJECT_PLUS_WINDOW_H
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>
#include <vector>
#include <QString>
#include <functional>
#include <QTextEdit>
#include <QFileDialog>
#include "../headers/Person.h"
#include "../headers/Database.h"
#include "../headers/Admin.h"
#include "../headers/Specialist.h"
#include "../headers/User.h"
#include "../headers/MyContainer.h"
class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(Database& db, QWidget* parent = nullptr);
    ~MainWindow();
private slots:
    void addNewClient();
    void createUser();
    void createSpecialist();
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
    void generateReport(User* user);
private:
    Database& db;
    QLabel* outputLabel;
    QVBoxLayout* mainLayout;
    MyContainer<Person*> people;
    Person* loggedInUser = nullptr;
    void mainMenu();
    void createMainMenuButtons();
    void clearLayout(QLayout* layout);
    void loadFromDatabase();
    void saveToDatabase();
    QLineEdit* createLineEdit(const QString& placeholder = "");
    QPushButton* createButton(const QString& text, std::function<void()> callback);
    void createPersonInputForm(const QString& type);
    std::vector<Specialist*> findMatchingSpecialists(User* user, const std::string& specialization);
    bool specializationMatches(const std::string& specialistSpecialization, const std::string& userSpecialization);
    bool addressMatches(const Address& userAddress, const Address& specialistAddress);
    bool areAllFieldsFilled(const QLineEdit* firstNameEdit, const QLineEdit* lastNameEdit, const QLineEdit* passwordEdit, const QLineEdit* countryEdit, const QLineEdit* regionEdit, const QLineEdit* cityEdit, const QLineEdit* streetEdit, const QLineEdit* houseEdit, const QLineEdit* apartmentEdit, const QLineEdit* contactEdit, const QLineEdit* specializationEdit = nullptr, const QLineEdit* certificationsEdit = nullptr) const;
    Person* getLoggedInUser() const;
    void displayClientDetailsInList(Person* person, QListWidget* listWidget);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    void resizeEvent(QResizeEvent* event) override;
};
#endif