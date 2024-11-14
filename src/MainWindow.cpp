#include "../headers/MainWindow.h"
#include "../headers/Address.h"
#include "../headers/User.h"
#include "../headers/Admin.h"
#include "../headers/Specialist.h"
#include "../headers/Database.h"
#include "../headers/exceptions.h"
#include "../headers/ReportGenerator.h"
#include "../headers/MyContainer.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>
#include <QString>
#include <sstream>
#include <functional>
#include <QPainter>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QApplication>
#include <QScreen>
#include <iomanip>
#include <algorithm>
MainWindow::MainWindow(Database& database, QWidget* parent) : QWidget(parent), db(database) {
    loadFromDatabase();
    setWindowTitle("Stuffing Service");
    QString imagePath = "C:/Users/micha/CLionProjects/StuffingService/resources/1111.png";
    backgroundImage.load(imagePath);
    outputLabel = new QLabel("Welcome to Stuffing service!", this);
    outputLabel->setWordWrap(true);
    outputLabel->setStyleSheet("color: white;");
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(outputLabel);
    QString buttonStyle = "QPushButton { background-color: purple; color: white; }";
    this->setStyleSheet(buttonStyle);
    mainMenu();
    setLayout(mainLayout);
}
void MainWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawPixmap(rect(), backgroundImage.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}
MainWindow::~MainWindow() {
    saveToDatabase();
    for (Person* p : people) {
        delete p;
    }
}
void MainWindow::mainMenu() {
    clearLayout(mainLayout);
    Person* loggedInUser = getLoggedInUser();
    Admin* admin = dynamic_cast<Admin*>(loggedInUser);
    if (admin) {
        mainLayout->addWidget(createButton("Display All Clients", [this]() { displayAllClients(); }));
    } else {
        createMainMenuButtons();
    }
}
void MainWindow::createMainMenuButtons() {
    mainLayout->addWidget(createButton("Add New Client", [this]() { addNewClient(); }));
    mainLayout->addWidget(createButton("Display All Clients", [this]() { displayAllClients(); }));
    mainLayout->addWidget(createButton("Find Info By Client Name", [this]() { findInfoByClientName(); }));
    mainLayout->addWidget(createButton("Login", [this]() { login(); }));
}
void MainWindow::clearLayout(QLayout* layout) {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget() && item->widget() != outputLabel) {
            delete item->widget();
        }
        delete item;
    }
}
void MainWindow::loadFromDatabase() {
    for (Person* p : people) {
        delete p;
    }
    people.clear();
    std::vector<Person*> tempPeople = db.loadFromDatabase(); 
    for(Person* p : tempPeople){
        people.add(p); 
    }
}
void MainWindow::saveToDatabase() {
    std::vector<Person*> tempPeople; 
    for(Person *p : people){
        tempPeople.push_back(p); 
    }
    db.saveToDatabase(tempPeople); 
}
QLineEdit* MainWindow::createLineEdit(const QString& placeholder) {
    QLineEdit* lineEdit = new QLineEdit(this);
    if (!placeholder.isEmpty()) {
        lineEdit->setPlaceholderText(placeholder);
    }
    return lineEdit;
}
QPushButton* MainWindow::createButton(const QString& text, std::function<void()> callback) {
    QPushButton* button = new QPushButton(text, this);
    QObject::connect(button, &QPushButton::clicked, callback);
    return button;
}
void MainWindow::addNewClient() {
    clearLayout(mainLayout);
    outputLabel->setText("Choose client type:");
    mainLayout->addWidget(createButton("User", [this]() { createUser(); }));
    mainLayout->addWidget(createButton("Specialist", [this]() { createSpecialist(); }));
    mainLayout->addWidget(createButton("Admin", [this]() { createAdmin(); }));
    mainLayout->addWidget(createButton("Back", [this]() { mainMenu(); }));
}
void MainWindow::createPersonInputForm(const QString& type) {
    clearLayout(mainLayout);
    outputLabel->setText("Creating a new " + type + ":");
    QLineEdit* firstNameEdit = createLineEdit("First Name");
    QLineEdit* lastNameEdit = createLineEdit("Last Name");
    QLineEdit* passwordEdit = createLineEdit("Password");
    QLineEdit* countryEdit = createLineEdit("Country");
    QLineEdit* regionEdit = createLineEdit("Region");
    QLineEdit* cityEdit = createLineEdit("City");
    QLineEdit* streetEdit = createLineEdit("Street");
    QLineEdit* houseEdit = createLineEdit("House");
    QLineEdit* apartmentEdit = createLineEdit("Apartment");
    QLineEdit* contactEdit = createLineEdit("Contact Info");
    QLineEdit* specializationEdit = nullptr;
    QLineEdit* certificationsEdit = nullptr;
    if (type == "Specialist") {
        specializationEdit = createLineEdit("Specialization");
        certificationsEdit = createLineEdit("Certifications (comma-separated)");
        mainLayout->addWidget(specializationEdit);
        mainLayout->addWidget(certificationsEdit);
    }
    mainLayout->addWidget(firstNameEdit);
    mainLayout->addWidget(lastNameEdit);
    mainLayout->addWidget(passwordEdit);
    mainLayout->addWidget(countryEdit);
    mainLayout->addWidget(regionEdit);
    mainLayout->addWidget(cityEdit);
    mainLayout->addWidget(streetEdit);
    mainLayout->addWidget(houseEdit);
    mainLayout->addWidget(apartmentEdit);
    mainLayout->addWidget(contactEdit);
    auto createPerson = [this, type, firstNameEdit, lastNameEdit, passwordEdit, countryEdit,
                         regionEdit, cityEdit, streetEdit, houseEdit, apartmentEdit, contactEdit,
                         specializationEdit, certificationsEdit]() {
        Address address(countryEdit->text().toStdString(), regionEdit->text().toStdString(),
                        cityEdit->text().toStdString(), streetEdit->text().toStdString(),
                        houseEdit->text().toStdString(), apartmentEdit->text().toStdString());
        Person* person = nullptr;
        if (type == "User") {
            person = new User(firstNameEdit->text().toStdString(), lastNameEdit->text().toStdString(),
                              passwordEdit->text().toStdString(), address, contactEdit->text().toStdString());
        } else if (type == "Specialist") {
            QStringList certificationsList = certificationsEdit->text().split(",");
            std::vector<std::string> certifications;
            for (const QString& cert : certificationsList) {
                certifications.push_back(cert.trimmed().toStdString());
            }
            person = new Specialist(firstNameEdit->text().toStdString(), lastNameEdit->text().toStdString(),
                                    passwordEdit->text().toStdString(), address, contactEdit->text().toStdString(),
                                    specializationEdit->text().toStdString(), certifications);
        } else if (type == "Admin") {
            person = new Admin(firstNameEdit->text().toStdString(), lastNameEdit->text().toStdString(),
                              passwordEdit->text().toStdString());
        }
        if (person != nullptr) {
            people.add(person);
            saveToDatabase();
            clearLayout(mainLayout);
            outputLabel->setText(type + " created successfully!");
            mainLayout->addWidget(createButton("Back", [this]() { mainMenu(); }));
        }
    };
    mainLayout->addWidget(createButton("Create", createPerson));
    mainLayout->addWidget(createButton("Back", [this]() { addNewClient(); }));
}
void MainWindow::createUser() {
    createPersonInputForm("User");
}
void MainWindow::createSpecialist() {
    createPersonInputForm("Specialist");
}
void MainWindow::createAdmin() {
    createPersonInputForm("Admin");
}
void MainWindow::displayAllClients() {
    loadFromDatabase();
    clearLayout(mainLayout);
    QListWidget* clientList = new QListWidget(this);
    clientList->setStyleSheet("background-color: lightblue; color: white;");
    for (const Person* person : people) {
        std::stringstream ss;
        ss << person->getFirstName() << " " << person->getLastName() << "\n";
        if (dynamic_cast<const User*>(person) || dynamic_cast<const Specialist*>(person)) {
            const Address& address = person->getAddress();
            ss << "   Country: " << address.getCountry() << "\n";
            ss << "   Region: " << address.getRegion() << "\n";
            ss << "   City: " << address.getCity() << "\n";
            ss << "   Street: " << address.getStreet() << "\n";
            ss << "   House: " << address.getHouse() << "\n";
            ss << "   Apartment: " << address.getApartment() << "\n";
            if (const User* user = dynamic_cast<const User*>(person)) {
                ss << "   Contact Info: " << user->getContactInfo() << "\n";
                if (!user->getRatings().empty()) {
                    double avgRating = user->getAverageRating();
                    ss << "   Average Rating: " << std::fixed << std::setprecision(1) << avgRating << "\n";
                    for (int rating : user->getRatings()) {
                        ss << "       Individual rating: " << rating << "\n";
                    }
                }
                const auto& reviews = user->getReviews();
                if (!reviews.empty()) {
                    for (const auto& review : reviews) {
                        ss << "      Review: " << review << "\n";
                    }
                }
            }
            if (const Specialist* specialist = dynamic_cast<const Specialist*>(person)) {
                ss << "   Contact Info: " << specialist->getContactInfo() << "\n";
                ss << "   Specialization: " << specialist->getSpecialization() << "\n";
                const auto& certifications = specialist->getCertifications();
                for (const auto& cert : certifications) {
                    ss << "       Certification: " << cert << "\n";
                }
                if (!specialist->getRatings().empty()) {
                    double avgRating = specialist->getAverageRating();
                    ss << "      Average Rating: " << std::fixed << std::setprecision(1) << avgRating << "\n";
                    for (int rating : specialist->getRatings()) {
                        ss << "          Individual rating: " << rating << "\n";
                    }
                }
                const auto& reviews = specialist->getReviews();
                if (!reviews.empty()) {
                    for (const auto& review : reviews) {
                        ss << "      Review: " << review << "\n";
                    }
                }
            }
        } else if (const Admin* admin = dynamic_cast<const Admin*>(person)) {
            const auto& privileges = admin->getPrivileges();
            if (!privileges.empty()) {
                for (const std::string& privilege : privileges) {
                    ss << "      Privilege: " << privilege << "\n";
                }
            }
        }
        new QListWidgetItem(QString::fromStdString(ss.str()), clientList);
    }
    QObject::connect(clientList, &QListWidget::itemClicked, this, &MainWindow::displayClientDetails);
    mainLayout->addWidget(clientList);
    mainLayout->addWidget(createButton("Back", [this]() { mainMenu(); }));
}
void MainWindow::displayClientDetails(QListWidgetItem* item) {
    QString clientName = item->text();
    QStringList nameParts = clientName.split(" ");
    if (nameParts.size() == 2) {
        QString firstName = nameParts[0];
        QString lastName = nameParts[1];
        loadFromDatabase();
        bool found = false;
        Person* foundPerson = nullptr;
        for (Person* person : people) {
            if (QString::fromStdString(person->getFirstName()) == firstName &&
                QString::fromStdString(person->getLastName()) == lastName) {
                foundPerson = person;
                found = true;
                break;
            }
        }
        if (found) {
            clearLayout(mainLayout);
            std::stringstream ss;
            ss << "First name: " << foundPerson->getFirstName() << std::endl;
            ss << "Last name: " << foundPerson->getLastName() << std::endl;
            ss << "Password: " << foundPerson->getPassword() << std::endl;
            ss << "Address: " << std::endl;
            ss << "  Country: " << foundPerson->getAddress().getCountry() << std::endl;
            ss << "  Region: " << foundPerson->getAddress().getRegion() << std::endl;
            ss << "  City: " << foundPerson->getAddress().getCity() << std::endl;
            ss << "  Street: " << foundPerson->getAddress().getStreet() << std::endl;
            ss << "  House: " << foundPerson->getAddress().getHouse() << std::endl;
            ss << "  Apartment: " << foundPerson->getAddress().getApartment() << std::endl;
            QWidget* infoContainer = new QWidget(this);
            infoContainer->setStyleSheet("background-color: lightblue;");
            QVBoxLayout* infoLayout = new QVBoxLayout(infoContainer);
            infoLayout->addWidget(new QLabel("User Information:", this));
            QLabel* userInfoLabel = new QLabel(QString::fromStdString(ss.str()), this);
            userInfoLabel->setStyleSheet("color: white;");
            userInfoLabel->setWordWrap(true);
            infoLayout->addWidget(userInfoLabel);
            int screenHeight = QGuiApplication::primaryScreen()->availableGeometry().height();
            infoContainer->setMaximumHeight(screenHeight / 2);
            infoContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            mainLayout->addWidget(infoContainer, 0, Qt::AlignCenter);
            mainLayout->addWidget(createButton("Back", [this]() { displayAllClients(); }));
        } else {
            clearLayout(mainLayout);
            outputLabel->setText("Client not found.");
            mainLayout->addWidget(createButton("Back", [this]() { displayAllClients(); }));
        }
    }
}
void MainWindow::findInfoByClientName() {
    clearLayout(mainLayout);
    outputLabel->setText("Enter client's first name:");
    QSpacerItem* spacer = new QSpacerItem(100, 100, QSizePolicy::Minimum, QSizePolicy::Fixed);
    mainLayout->addItem(spacer);
    QLineEdit* firstNameEdit = createLineEdit();
    mainLayout->addWidget(firstNameEdit);
    mainLayout->addWidget(new QLabel("Enter client's last name:", this));
    QSpacerItem* spacerLow = new QSpacerItem(100, 100, QSizePolicy::Minimum, QSizePolicy::Fixed);
    mainLayout->addItem(spacerLow);
    QLineEdit* lastNameEdit = createLineEdit();
    mainLayout->addWidget(lastNameEdit);
    mainLayout->addWidget(createButton("Search", [this, firstNameEdit, lastNameEdit]() {
        QString firstName = firstNameEdit->text();
        QString lastName = lastNameEdit->text();
        try {
            bool userFound = false;
            Person* foundPerson = nullptr;
            for (Person* p : people) {
                if (QString::fromStdString(p->getFirstName()) == firstName && QString::fromStdString(p->getLastName()) == lastName) {
                    userFound = true;
                    foundPerson = p;
                    break;
                }
            }
            if (!userFound) {
                throw UserNotFoundException("User not found.");
            }
            clearLayout(mainLayout);
            std::stringstream ss;
            ss << "First name: " << foundPerson->getFirstName() << std::endl;
            ss << "Last name: " << foundPerson->getLastName() << std::endl;
            if (const User* user = dynamic_cast<const User*>(foundPerson)) {
                ss << "Address: " << user->getAddress().getCountry() << ", " << user->getAddress().getRegion() << ", " << user->getAddress().getCity() << ", " << user->getAddress().getStreet() << ", " << user->getAddress().getHouse() << ", " << user->getAddress().getApartment() << std::endl;
                if (user->getAverageRating() > 0) {
                    ss << "Rating: " << user->getAverageRating() << std::endl;
                }
                for (const std::string& review : user->getReviews()) {
                    ss << "Review: " << review << std::endl;
                }
                ss << "Contact Info: " << user->getContactInfo() << std::endl;
            }
            if (const Specialist* specialist = dynamic_cast<const Specialist*>(foundPerson)) {
                ss << "Specialization: " << specialist->getSpecialization() << std::endl;
                for (const auto& cert : specialist->getCertifications()) {
                    ss << "Certification: " << cert << std::endl;
                }
            }
            if (const Admin* admin = dynamic_cast<const Admin*>(foundPerson)) {
                for (const std::string& privilege : admin->getPrivileges()) {
                    ss << "Privilege: " << privilege << std::endl;
                }
            }
            outputLabel->setText(QString::fromStdString(ss.str()));
        } catch (const UserNotFoundException& ex) {
            QMessageBox::warning(this, "Error", ex.what());
        }
        mainLayout->addWidget(createButton("Back", [this]() { findInfoByClientName(); }));
    }));
    mainLayout->addWidget(createButton("Back", [this]() { mainMenu(); }));
}
void MainWindow::login() {
    clearLayout(mainLayout);
    outputLabel->setText("Enter your first name:");
    QSpacerItem* spacer = new QSpacerItem(100, 100, QSizePolicy::Minimum, QSizePolicy::Fixed);
    mainLayout->addItem(spacer);
    QLineEdit* firstNameEdit = createLineEdit();
    mainLayout->addWidget(firstNameEdit);
    mainLayout->addWidget(new QLabel("Enter your last name:", this));
    QLineEdit* lastNameEdit = createLineEdit();
    mainLayout->addWidget(lastNameEdit);
    mainLayout->addWidget(createButton("Login", [this, firstNameEdit, lastNameEdit]() {
         QString firstName = firstNameEdit->text();
         QString lastName = lastNameEdit->text();
         try {
             for (Person* person : people) {
                 if (QString::fromStdString(person->getFirstName()) == firstName &&
                     QString::fromStdString(person->getLastName()) == lastName) {
                     bool ok;
                     QString password = QInputDialog::getText(this, "Login", "Enter password:",
                                                             QLineEdit::Password, QString(), &ok);
                     if (ok) {
                         if (QString::fromStdString(person->getPassword()) == password) {
                             handleLogin(person);
                             return;
                         } else {
                             throw IncorrectPasswordException("Incorrect password.");
                         }
                     } else {
                        return;
                     }
                 }
             }
             throw UserNotFoundException("User not found.");
         } catch (const UserNotFoundException& ex) {
             QMessageBox::warning(this, "Login Error", ex.what());
         } catch (const IncorrectPasswordException& ex) {
             QMessageBox::warning(this, "Login Error", ex.what());
         }
     }));
    mainLayout->addWidget(createButton("Back", [this]() { mainMenu(); }));
}
Person* MainWindow::getLoggedInUser() const {
    return loggedInUser;
}
void MainWindow::handleLogin(Person* person) {
    clearLayout(mainLayout);
    loggedInUser = person;
    QString profile = "Welcome, " + QString::fromStdString(person->getFirstName()) + "!\n\n";
    User* user = dynamic_cast<User*>(person);
    Specialist* specialist = dynamic_cast<Specialist*>(person);
    Admin* admin = dynamic_cast<Admin*>(person);
    mainLayout->addWidget(createButton("View Profile", [this, person, profile]() {
        clearLayout(mainLayout);
        std::stringstream ss;
        ss << person;
        outputLabel->setText(profile + QString::fromStdString(ss.str()));
        mainLayout->addWidget(createButton("Back", [this, person]() { handleLogin(person); }));
    }));
    mainLayout->addWidget(createButton("Edit Profile", [this, person]() { editProfile(person); }));
    mainLayout->addWidget(createButton("Delete Account", [this, person]() { deleteAccount(person); }));
    if (user) {
        mainLayout->addWidget(createButton("Rate and Review Specialist", [this, user]() { rateAndReviewSpecialist(user); }));
        mainLayout->addWidget(createButton("Find Specialist", [this, user]() { generateReport(user); }));
    } else if (specialist) {
        mainLayout->addWidget(createButton("Rate and Review User", [this, specialist]() { rateAndReviewUser(specialist); }));
    } else if (admin) {
        mainLayout->addWidget(createButton("Delete User by Name", [this, admin]() { deleteUserByName(admin); }));
        profile += "Admin Privileges:\n";
        for (const auto& privilege : admin->getPrivileges()) {
            profile += QString::fromStdString(privilege) + "\n";
        }
    }
    outputLabel->setText(profile);
    mainLayout->addWidget(createButton("Logout", [this]() { logout(); }));
}
void MainWindow::editProfile(Person* person) {
    clearLayout(mainLayout);
    outputLabel->setText("Edit your profile:");
    QLineEdit* firstNameEdit = createLineEdit(QString::fromStdString(person->getFirstName()));
    QLineEdit* lastNameEdit = createLineEdit(QString::fromStdString(person->getLastName()));
    QLineEdit* passwordEdit = createLineEdit("New Password");
    mainLayout->addWidget(firstNameEdit);
    mainLayout->addWidget(lastNameEdit);
    mainLayout->addWidget(passwordEdit);
    auto saveChanges = [this, person, firstNameEdit, lastNameEdit, passwordEdit]() {
        person->setFirstName(firstNameEdit->text().toStdString());
        person->setLastName(lastNameEdit->text().toStdString());
        if (!passwordEdit->text().isEmpty()) {
            person->setPassword(passwordEdit->text().toStdString());
        }
        saveToDatabase();
        clearLayout(mainLayout);
        outputLabel->setText("Profile updated successfully!");
        mainLayout->addWidget(createButton("Back", [this, person]() { handleLogin(person); }));
    };
    mainLayout->addWidget(createButton("Save Changes", saveChanges));
    mainLayout->addWidget(createButton("Back", [this, person]() { handleLogin(person); }));
}
void MainWindow::deleteAccount(Person* person) {
    clearLayout(mainLayout);
    outputLabel->setText("Are you sure you want to delete your account?");
    mainLayout->addWidget(createButton("Confirm", [this, person]() {
        for (auto it = people.begin(); it != people.end(); ++it) {
            if (*it == person) {
                delete *it;
                people.remove(person);
                saveToDatabase();
                clearLayout(mainLayout);
                outputLabel->setText("Account deleted successfully.");
                mainLayout->addWidget(createButton("Back", [this]() { mainMenu(); }));
                return;
            }
        }
    }));
    mainLayout->addWidget(createButton("Cancel", [this, person]() { handleLogin(person); }));
}
void MainWindow::rateAndReviewUser(Specialist* specialist) {
    clearLayout(mainLayout);
    outputLabel->setText("Rate and review a user:");
    QLineEdit* userFirstNameEdit = createLineEdit("User First Name");
    QLineEdit* userLastNameEdit = createLineEdit("User Last Name");
    QLineEdit* ratingEdit = createLineEdit("Rating (1-5)");
    QLineEdit* reviewEdit = createLineEdit("Review");
    mainLayout->addWidget(userFirstNameEdit);
    mainLayout->addWidget(userLastNameEdit);
    mainLayout->addWidget(ratingEdit);
    mainLayout->addWidget(reviewEdit);
    auto submitReview = [this, specialist, userFirstNameEdit, userLastNameEdit, ratingEdit, reviewEdit]() {
        QString userFirstName = userFirstNameEdit->text();
        QString userLastName = userLastNameEdit->text();
        loadFromDatabase();
        bool userFound = false;
        User* foundUser = nullptr;
        for (Person* p : people) {
            User* u = dynamic_cast<User*>(p);
            if (u && QString::fromStdString(u->getFirstName()) == userFirstName && QString::fromStdString(u->getLastName()) == userLastName) {
                userFound = true;
                foundUser = u;
                break;
            }
        }
        if (!userFound) {
            QMessageBox::warning(this, "Error", "User not found.");
            return;
        }
        if (foundUser) {
            bool ok;
            int rating = ratingEdit->text().toInt(&ok);
            if (ok && rating >= 1 && rating <= 5) {
                foundUser->addRating(rating);
                foundUser->addReview(reviewEdit->text().toStdString());
                saveToDatabase();
                clearLayout(mainLayout);
                outputLabel->setText("Rating and review submitted successfully!");
                mainLayout->addWidget(createButton("Back", [this, specialist]() { handleLogin(specialist); }));
            } else {
                QMessageBox::warning(this, "Error", "Invalid rating. Please enter a number between 1 and 5.");
            }
        }
    };
    mainLayout->addWidget(createButton("Submit", submitReview));
    mainLayout->addWidget(createButton("Back", [this, specialist]() { handleLogin(specialist); }));
}
void MainWindow::rateAndReviewSpecialist(User* user) {
    clearLayout(mainLayout);
    outputLabel->setText("Rate and review a specialist:");
    QLineEdit* specialistFirstNameEdit = createLineEdit("Specialist First Name");
    QLineEdit* specialistLastNameEdit = createLineEdit("Specialist Last Name");
    QLineEdit* ratingEdit = createLineEdit("Rating (1-5)");
    QLineEdit* reviewEdit = createLineEdit("Review");
    mainLayout->addWidget(specialistFirstNameEdit);
    mainLayout->addWidget(specialistLastNameEdit);
    mainLayout->addWidget(ratingEdit);
    mainLayout->addWidget(reviewEdit);
    auto submitReview = [this, user, specialistFirstNameEdit, specialistLastNameEdit, ratingEdit, reviewEdit]() {
        QString specialistFirstName = specialistFirstNameEdit->text();
        QString specialistLastName = specialistLastNameEdit->text();
        bool specialistFound = false;
        Specialist *foundSpecialist = nullptr;
        for(Person *p : people) {
            Specialist *s = dynamic_cast<Specialist*>(p);
            if (s && QString::fromStdString(s->getFirstName()) == specialistFirstName && QString::fromStdString(s->getLastName()) == specialistLastName){
                specialistFound = true;
                foundSpecialist = s;
                break;
            }
        }
        if (!specialistFound) {
            QMessageBox::warning(this, "Error", "Specialist not found.");
            return;
        }
        if (foundSpecialist) {
            bool ok;
            int rating = ratingEdit->text().toInt(&ok);
            if (ok && rating >= 1 && rating <= 5) {
                foundSpecialist->addRating(rating);
                foundSpecialist->addReview(reviewEdit->text().toStdString());
                saveToDatabase();
                clearLayout(mainLayout);
                outputLabel->setText("Rating and review submitted successfully!");
                mainLayout->addWidget(createButton("Back", [this, user]() { handleLogin(user); }));
            } else {
                QMessageBox::warning(this, "Error", "Invalid rating. Please enter a number between 1 and 5.");
            }
        }
    };
    mainLayout->addWidget(createButton("Submit", submitReview));
    mainLayout->addWidget(createButton("Back", [this, user]() { handleLogin(user); }));
}
void MainWindow::deleteUserByName(Admin* admin) {
    clearLayout(mainLayout);
    outputLabel->setText("Enter client's first name:");
    QLineEdit* firstNameEdit = createLineEdit();
    mainLayout->addWidget(firstNameEdit);
    mainLayout->addWidget(new QLabel("Enter client's last name:", this));
    QLineEdit* lastNameEdit = createLineEdit();
    mainLayout->addWidget(lastNameEdit);
    QPushButton* deleteButton = createButton("Delete", [this, firstNameEdit, lastNameEdit, admin]() {
        QString firstName = firstNameEdit->text();
        QString lastName = lastNameEdit->text();
        auto it = people.begin();
        int index = 0;
        bool userFound = false;
        for (; it != people.end(); ++it, index++) {
            if (QString::fromStdString((*it)->getFirstName()) == firstName && QString::fromStdString((*it)->getLastName()) == lastName) {
                userFound = true;
                break;
            }
        }
        if (userFound) {
            delete people[index];
            people.remove(*it);
            saveToDatabase();
            clearLayout(mainLayout);
            outputLabel->setText("User deleted successfully.");
        } else {
            clearLayout(mainLayout);
            outputLabel->setText("User not found.");
        }
        mainLayout->addWidget(createButton("Back", [this, admin]() { deleteUserByName(admin); }));
    });
    mainLayout->addWidget(deleteButton);
    mainLayout->addWidget(createButton("Back", [this, admin]() { handleLogin(admin); }));
}
std::vector<Specialist*> MainWindow::findMatchingSpecialists(User* user, const std::string& specialization) {
    std::vector<Specialist*> matchingSpecialists;
    for (size_t i = 0; i < people.size(); ++i) {
        Specialist* specialist = dynamic_cast<Specialist*>(people[i]);
        if (specialist && specializationMatches(specialist->getSpecialization(), specialization) && addressMatches(user->getAddress(), specialist->getAddress())) {
            matchingSpecialists.push_back(specialist);
        }
    }
    return matchingSpecialists;
}
void MainWindow::logout() {
    loggedInUser = nullptr;
    mainMenu();
}
void MainWindow::generateReport(User *user) {
    clearLayout(mainLayout);
    outputLabel->setText("Generate Specialist Report:");
    QLineEdit* specializationEdit = createLineEdit("Specialization");
    mainLayout->addWidget(specializationEdit);
    mainLayout->addWidget(createButton("Generate report", [this, user, specializationEdit]() {
        std::string specialization = specializationEdit->text().toStdString();
        ReportGenerator reportGenerator;
        std::vector<Specialist*> matchingSpecialists = findMatchingSpecialists(user, specialization); 
        QString report = QString::fromStdString(reportGenerator.generateXMLReport(matchingSpecialists));
        QTextEdit* reportTextEdit = new QTextEdit(this);
        reportTextEdit->setPlainText(report);
        reportTextEdit->setReadOnly(true);
        clearLayout(mainLayout);
        mainLayout->addWidget(reportTextEdit);
        mainLayout->addWidget(createButton("Save Report", [report]() {
            QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Report", "", "XML Files (*.xml);;All Files (*)");
            if (!fileName.isEmpty()) {
                QFile file(fileName);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&file);
                    out << report;
                    file.close();
                }
            }
        }));
        mainLayout->addWidget(createButton("Back", [this, user]() { handleLogin(user); }));
    }));
    mainLayout->addWidget(createButton("Back", [this, user]() { handleLogin(user); }));
}
bool MainWindow::specializationMatches(const std::string& specialistSpecialization, const std::string& userSpecialization) {
    std::string lowerSpec = specialistSpecialization;
    std::transform(lowerSpec.begin(), lowerSpec.end(), lowerSpec.begin(), ::tolower);
    std::string lowerUser = userSpecialization;
    std::transform(lowerUser.begin(), lowerUser.end(), lowerUser.begin(), ::tolower);
    if (lowerSpec.find(lowerUser) != std::string::npos) return true;
    int diffCount = 0;
    int minLength = std::min(lowerSpec.length(), lowerUser.length());
    for (int i = 0; i < minLength; ++i) {
        if (lowerSpec[i] != lowerUser[i]) {
            diffCount++;
        }
    }
    return diffCount < 3;
}
bool MainWindow::addressMatches(const Address& userAddress, const Address& specialistAddress) {
    return (userAddress.getCountry() == specialistAddress.getCountry() ||
            userAddress.getRegion() == specialistAddress.getRegion() ||
            userAddress.getCity() == specialistAddress.getCity() ||
            userAddress.getStreet() == specialistAddress.getStreet() ||
            userAddress.getHouse() == specialistAddress.getHouse() ||
            userAddress.getApartment() == specialistAddress.getApartment());
}