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
        mainLayout->addWidget(createButton("Find Info By Client Name", [this]() { findInfoByClientName(); }));
    } else {
        createMainMenuButtons();
    }
}

void MainWindow::createMainMenuButtons() {
    mainLayout->addWidget(createButton("Add New Client", [this]() { addNewClient(); }));
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
    for (Person* p : tempPeople) {
        people.add(p);
    }
}

void MainWindow::saveToDatabase() {
    std::vector<Person*> tempPeople;
    for (Person* p : people) {
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

        if (!areAllFieldsFilled(firstNameEdit, lastNameEdit, passwordEdit, countryEdit, regionEdit, cityEdit, streetEdit, houseEdit, apartmentEdit, contactEdit, specializationEdit, certificationsEdit)) {
            QMessageBox::warning(this, "Error", "Please fill in all fields.");
            return;
        }

        Address address(countryEdit->text().toStdString(), regionEdit->text().toStdString(),
                        cityEdit->text().toStdString(), streetEdit->text().toStdString(),
                        houseEdit->text().toStdString(), apartmentEdit->text().toStdString());
        Person* person = nullptr;

        try {
            if (!db.isPasswordUnique(passwordEdit->text().toStdString())) {
                throw PasswordNotUniqueException("Password already exists. Please choose a different password.");
            }

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
            }

            if (person != nullptr) {
                people.add(person);
                saveToDatabase();
                clearLayout(mainLayout);
                outputLabel->setText(type + " created successfully!");
                mainLayout->addWidget(createButton("Back", [this]() { mainMenu(); }));
            }
        } catch (const PasswordNotUniqueException& e) {
            QMessageBox::warning(this, "Error", e.what());
            delete person;  
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
        }

        new QListWidgetItem(QString::fromStdString(ss.str()), clientList);
    }

    QObject::connect(clientList, &QListWidget::itemClicked, this, &MainWindow::displayClientDetails);
    mainLayout->addWidget(clientList);
    mainLayout->addWidget(createButton("Back", [this]() {
        if (loggedInUser && dynamic_cast<Admin*>(loggedInUser)) {
            handleLogin(loggedInUser); 
        } else {
            mainMenu(); 
        }
    }));
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
            QTextEdit* detailsTextEdit = new QTextEdit(this);
            detailsTextEdit->setReadOnly(true);
            detailsTextEdit->setStyleSheet("background-color: lightblue; color: white;");
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

            if (const User* user = dynamic_cast<const User*>(foundPerson)) {
                ss << "Contact Info: " << user->getContactInfo() << std::endl;
                if (!user->getRatings().empty()) {
                    double avgRating = user->getAverageRating();
                    ss << "Average Rating: " << std::fixed << std::setprecision(1) << avgRating << std::endl;
                    for (int rating : user->getRatings()) {
                        ss << "Individual Rating: " << rating << std::endl;
                    }
                }
                for (const std::string& review : user->getReviews()) {
                    ss << "Review: " << review << std::endl;
                }
            } else if (const Specialist* specialist = dynamic_cast<const Specialist*>(foundPerson)) {
                ss << "Contact Info: " << specialist->getContactInfo() << std::endl;
                ss << "Specialization: " << specialist->getSpecialization() << std::endl;
                for (const std::string& cert : specialist->getCertifications()) {
                    ss << "Certification: " << cert << std::endl;
                }
                if (!specialist->getRatings().empty()) {
                    double avgRating = specialist->getAverageRating();
                    ss << "Average Rating: " << std::fixed << std::setprecision(1) << avgRating << std::endl;
                    for (int rating : specialist->getRatings()) {
                        ss << "Individual Rating: " << rating << std::endl;
                    }
                }
                for (const std::string& review : specialist->getReviews()) {
                    ss << "Review: " << review << std::endl;
                }
            }
            detailsTextEdit->setPlainText(QString::fromStdString(ss.str()));
            mainLayout->addWidget(detailsTextEdit);
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

    QLineEdit* firstNameEdit = createLineEdit();
    mainLayout->addWidget(firstNameEdit);

    mainLayout->addWidget(new QLabel("Enter client's last name:", this));
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
            QTextEdit* detailsTextEdit = new QTextEdit(this);
            detailsTextEdit->setReadOnly(true);
            detailsTextEdit->setStyleSheet("background-color: lightblue; color: white;");
            std::stringstream ss;
            ss << *foundPerson;
            detailsTextEdit->setPlainText(QString::fromStdString(ss.str()));
            mainLayout->addWidget(detailsTextEdit);
            mainLayout->addWidget(createButton("Back", [this]() {
                if (loggedInUser && dynamic_cast<Admin*>(loggedInUser)) {
                    handleLogin(loggedInUser);
                } else {
                    mainMenu();
                }
            }));

        } catch (const UserNotFoundException& ex) {
            QMessageBox::warning(this, "Error", ex.what());
            mainLayout->addWidget(createButton("Back", [this]() { findInfoByClientName(); }));
        }
    }));

    mainLayout->addWidget(createButton("Back", [this]() {
        if (loggedInUser && dynamic_cast<Admin*>(loggedInUser)) {
            handleLogin(loggedInUser);
        } else {
            mainMenu();
        }
    }));
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

    if (user || specialist) { 
        mainLayout->addWidget(createButton("View Profile", [this, person, profile]() {
            clearLayout(mainLayout);
            std::stringstream ss;
            ss << *person;
            outputLabel->setText(profile + QString::fromStdString(ss.str()));
            mainLayout->addWidget(createButton("Back", [this, person]() { handleLogin(person); }));
        }));

        mainLayout->addWidget(createButton("Edit Profile", [this, person]() { editProfile(person); }));
        mainLayout->addWidget(createButton("Delete Account", [this, person]() { deleteAccount(person); }));
    }

    if (user) {
        mainLayout->addWidget(createButton("Rate and Review Specialist", [this, user]() { rateAndReviewSpecialist(user); }));
        mainLayout->addWidget(createButton("Find Specialist", [this, user]() { generateReport(user); }));
    } else if (specialist) {
        mainLayout->addWidget(createButton("Rate and Review User", [this, specialist]() { rateAndReviewUser(specialist); }));
    }

    if (admin) { 
        mainLayout->addWidget(createButton("Delete User by Name", [this, admin]() { deleteUserByName(admin); }));
        mainLayout->addWidget(createButton("Display All Clients", [this]() { displayAllClients(); }));
        mainLayout->addWidget(createButton("Find Info By Client Name", [this]() { findInfoByClientName(); }));
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
        try {
            if (!db.isPasswordUnique(passwordEdit->text().toStdString()) && !passwordEdit->text().isEmpty()) {
                throw PasswordNotUniqueException("Password already exists. Please choose a different one.");
            }
            if (firstNameEdit->text().isEmpty() || lastNameEdit->text().isEmpty()) {
                throw EmptyFieldException("Name fields must not be empty.");
            }

            person->setFirstName(firstNameEdit->text().toStdString());
            person->setLastName(lastNameEdit->text().toStdString());
            if (!passwordEdit->text().isEmpty()) {
                person->setPassword(passwordEdit->text().toStdString());
            }

            saveToDatabase();
            clearLayout(mainLayout);
            outputLabel->setText("Profile updated successfully!");

            
            if (Admin* admin = dynamic_cast<Admin*>(person)) {
                handleLogin(admin);
            } else {
                handleLogin(person);
            }
        } catch (const PasswordNotUniqueException& ex) {
            QMessageBox::warning(this, "Error", ex.what());
        } catch (const EmptyFieldException& ex) {
            QMessageBox::warning(this, "Error", ex.what());
        }
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
                loggedInUser = nullptr; 
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
            foundUser = dynamic_cast<User*>(p); 
            if (foundUser && QString::fromStdString(foundUser->getFirstName()) == userFirstName && QString::fromStdString(foundUser->getLastName()) == userLastName) {
                userFound = true;
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

        loadFromDatabase();

        bool specialistFound = false;
        Specialist* foundSpecialist = nullptr;

        for (Person* p : people) {
            foundSpecialist = dynamic_cast<Specialist*>(p);
            if (foundSpecialist && QString::fromStdString(foundSpecialist->getFirstName()) == specialistFirstName &&
                QString::fromStdString(foundSpecialist->getLastName()) == specialistLastName) {
                specialistFound = true;
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

        bool userFound = false;
        for (auto it = people.begin(); it != people.end(); ++it) {
            if (QString::fromStdString((*it)->getFirstName()) == firstName && QString::fromStdString((*it)->getLastName()) == lastName) {
                
                people.remove(*it);
                userFound = true;
                break;
            }
        }

        if (userFound) {
            saveToDatabase();
            clearLayout(mainLayout);
            outputLabel->setText("User deleted successfully.");
            mainLayout->addWidget(createButton("Back", [this, admin]() { handleLogin(admin); }));
        } else {
            clearLayout(mainLayout);
            outputLabel->setText("User not found.");
            mainLayout->addWidget(createButton("Back", [this, admin]() { handleLogin(admin); }));
        }
    });

    mainLayout->addWidget(deleteButton);
    mainLayout->addWidget(createButton("Back", [this, admin]() { handleLogin(admin); }));
}

void MainWindow::generateReport(User* user) {
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

        mainLayout->addWidget(createButton("Save Report", [report, this]() {
            QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Report", "", "XML Files (.xml);;All Files ()");
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

std::vector<Specialist*> MainWindow::findMatchingSpecialists(User* user, const std::string& specialization) {
    std::vector<Specialist*> matchingSpecialists;
    for (Person* p : people) {
        Specialist* specialist = dynamic_cast<Specialist*>(p);
        if (specialist && specializationMatches(specialist->getSpecialization(), specialization) &&
            addressMatches(user->getAddress(), specialist->getAddress())) {
            matchingSpecialists.push_back(specialist);
            }
    }
    return matchingSpecialists;
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

bool MainWindow::areAllFieldsFilled(const QLineEdit* firstNameEdit, const QLineEdit* lastNameEdit, const QLineEdit* passwordEdit, const QLineEdit* countryEdit, const QLineEdit* regionEdit, const QLineEdit* cityEdit, const QLineEdit* streetEdit, const QLineEdit* houseEdit, const QLineEdit* apartmentEdit, const QLineEdit* contactEdit, const QLineEdit* specializationEdit, const QLineEdit* certificationsEdit) const {
    if (firstNameEdit->text().isEmpty() || lastNameEdit->text().isEmpty() || passwordEdit->text().isEmpty() ||
        countryEdit->text().isEmpty() || regionEdit->text().isEmpty() || cityEdit->text().isEmpty() ||
        streetEdit->text().isEmpty() || houseEdit->text().isEmpty() || apartmentEdit->text().isEmpty() ||
        contactEdit->text().isEmpty()) {
        return false;
        }

    if (specializationEdit && certificationsEdit && (specializationEdit->text().isEmpty() || certificationsEdit->text().isEmpty())) {
        return false;
    }

    return true;
}
void MainWindow::logout() {
    loggedInUser = nullptr;
    mainMenu();
}