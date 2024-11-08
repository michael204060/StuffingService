#include "../headers/MainWindow.h" 
#include "../headers/Address.h"
#include "../headers/User.h"
#include "../headers/Admin.h"
#include "../headers/Specialist.h"
#include "../headers/Database.h"
#include "../headers/exceptions.h"
#include "../headers/ReportGenerator.h"

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

MainWindow::MainWindow(Database& database, QWidget* parent) : QWidget(parent), db(database) {
    loadFromDatabase();
    setWindowTitle("Stuffing Service");

    
    QString imagePath = "C:/Users/micha/CLionProjects/StuffingService/resources/1111.png"; 
    backgroundImage.load(imagePath); 

    outputLabel = new QLabel("Welcome to Stuffing service!", this);
    outputLabel->setWordWrap(true);
    outputLabel->setStyleSheet("color: red;");

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(outputLabel);

    
    QString buttonStyle = "QPushButton { background-color: purple; color: red; }";
    
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
    people.clear();
}

void MainWindow::mainMenu() {
    clearLayout(mainLayout);
    createMainMenuButtons();
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
            item->widget()->deleteLater();
        }
        delete item;
    }
}

void MainWindow::loadFromDatabase() {
    for (Person* p : people) {
        delete p;
    }
    people.clear();
    people = db.loadFromDatabase();
}

void MainWindow::saveToDatabase() {
    db.saveToDatabase(people);
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
    QObject::connect(button, &QPushButton::clicked, this, callback);
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
            people.push_back(person);
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
    clientList->setStyleSheet("background-color: lightblue; color: red;");

    for (const Person* person : people) {
        std::stringstream ss;
        ss << person->getFirstName() << " " << person->getLastName() << "\n";

        
        const Address& address = person->getAddress();
        ss << "Address: " << address.getCountry() << ", "
           << address.getRegion() << ", " << address.getCity() << "\n";


        const User* user = dynamic_cast<const User*>(person);
        if (user) {
            double avgRating = user->getAverageRating();
            if (avgRating > 0) {
                ss << "Rating: " << avgRating << "\n";
            }
        }

        
        const Specialist* specialist = dynamic_cast<const Specialist*>(person);
        if (specialist) {
            ss << "Specialization: " << specialist->getSpecialization() << "\n";
            std::cout << "Specialization: " << specialist->getSpecialization() << "\n";
            
            for (const auto& cert : specialist->getCertifications()) {
                ss << "Certification: " << cert << "\n";
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
        auto it = std::find_if(people.begin(), people.end(), [&](const Person* person) {
            return QString::fromStdString(person->getFirstName()) == firstName &&
                   QString::fromStdString(person->getLastName()) == lastName;
        });

        if (it != people.end()) {
            clearLayout(mainLayout);

            std::stringstream ss;

            ss << "First name: " << (*it)->getFirstName() << std::endl;
            std::cout << "First name: " << (*it)->getFirstName() << "\n";
            ss << "Last name: " << (*it)->getLastName() << std::endl;
            ss << "Password: " << (*it)->getPassword() << std::endl;
            ss << "Address: " << std::endl;
            ss << "  Country: " << (*it)->getAddress().getCountry() << std::endl;
            ss << "  Region: " << (*it)->getAddress().getRegion() << std::endl;
            ss << "  City: " << (*it)->getAddress().getCity() << std::endl;
            ss << "  Street: " << (*it)->getAddress().getStreet() << std::endl;
            ss << "  House: " << (*it)->getAddress().getHouse() << std::endl;
            ss << "  Apartment: " << (*it)->getAddress().getApartment() << std::endl;

            
            QWidget* infoContainer = new QWidget(this);
            infoContainer->setStyleSheet("background-color: lightblue;"); 
            QVBoxLayout* infoLayout = new QVBoxLayout(infoContainer);
            infoLayout->addWidget(new QLabel("User Information:", this)); 

            
            QLabel* userInfoLabel = new QLabel(QString::fromStdString(ss.str()), this);
            userInfoLabel->setStyleSheet("color: red;"); 
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
            auto it = std::find_if(people.begin(), people.end(), [&](const Person* person) {
                return QString::fromStdString(person->getFirstName()) == firstName &&
                       QString::fromStdString(person->getLastName()) == lastName;
            });

            if (it == people.end()) {
                throw UserNotFoundException("User not found.");
            }

            clearLayout(mainLayout);
            std::stringstream ss;
            Person* person = *it;

            ss << "First name: " << person->getFirstName() << std::endl;
            ss << "Last name: " << person->getLastName() << std::endl;

            if (const User* user = dynamic_cast<const User*>(person)) {
                ss << "Address: " << user->getAddress().getCountry() << ", "
                   << user->getAddress().getRegion() << ", "
                   << user->getAddress().getCity() << ", "
                   << user->getAddress().getStreet() << ", "
                   << user->getAddress().getHouse() << ", "
                   << user->getAddress().getApartment() << std::endl;

                if (user->getAverageRating() > 0) {
                    ss << "Rating: " << user->getAverageRating() << std::endl;
                }

                for (const std::string& review : user->getReviews()) {
                    ss << "Review: " << review << std::endl;
                }

                ss << "Contact Info: " << user->getContactInfo() << std::endl;
            }

            if (const Specialist* specialist = dynamic_cast<const Specialist*>(person)) {
                ss << "Specialization: " << specialist->getSpecialization() << std::endl;
                for (const auto& cert : specialist->getCertifications()) {
                    ss << "Certification: " << cert << std::endl;
                }
            }

            if (const Admin* admin = dynamic_cast<const Admin*>(person)) {
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

void MainWindow::handleLogin(Person* person) {
    clearLayout(mainLayout);
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

    mainLayout->addWidget(createButton("Edit Profile", [this, person]() {
        editProfile(person);
    }));

    mainLayout->addWidget(createButton("Delete Account", [this, person]() {
        deleteAccount(person);
    }));

    if (user) {
        mainLayout->addWidget(createButton("Rate and Review Specialist", [this, user]() {
            rateAndReviewSpecialist(user);


        }));
        mainLayout->addWidget(createButton("Find Specialist", [this, user]() { generateReport(user); })); 


    } else if (specialist) {
        mainLayout->addWidget(createButton("Rate and Review User", [this, specialist]() {
            rateAndReviewUser(specialist);
        }));
    } else if (admin) {
        mainLayout->addWidget(createButton("Delete User by Name", [this, admin]() {
            deleteUserByName(admin);
        }));
        profile += "Admin Privileges:\n";
        for (const auto& privilege : admin->getPrivileges()) {
            profile += QString::fromStdString(privilege) + "\n";
        }
    }

    outputLabel->setText(profile);
    mainLayout->addWidget(createButton("Logout", [this]() { logout(); }));
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


std::vector<Specialist*> MainWindow::findMatchingSpecialists(User* user, const std::string& specialization) {
    std::vector<Specialist*> matchingSpecialists;

    for (Person* person : people) {
        Specialist* specialist = dynamic_cast<Specialist*>(person);
        if (specialist) {
            if (specializationMatches(specialist->getSpecialization(), specialization) &&
                addressMatches(user->getAddress(), specialist->getAddress())) {
                matchingSpecialists.push_back(specialist);
            }
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
        auto it = std::find(people.begin(), people.end(), person);
        if (it != people.end()) {
            delete *it;
            people.erase(it);
            saveToDatabase();
            clearLayout(mainLayout);
            outputLabel->setText("Account deleted successfully.");
            mainLayout->addWidget(createButton("Back", [this]() { mainMenu(); }));
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
        auto it = std::find_if(people.begin(), people.end(), [&](const Person* p) {
            return QString::fromStdString(p->getFirstName()) == userFirstName &&
                   QString::fromStdString(p->getLastName()) == userLastName;
        });

        if (it != people.end()) {
            User* user = dynamic_cast<User*>(*it);
            if (user) {
                bool ok;
                int rating = ratingEdit->text().toInt(&ok);
                if (ok && rating >= 1 && rating <= 5) {
                    user->addRating(rating);
                    user->addReview(reviewEdit->text().toStdString());
                    saveToDatabase();
                    clearLayout(mainLayout);
                    outputLabel->setText("Rating and review submitted successfully!");
                    mainLayout->addWidget(createButton("Back", [this, specialist]() { handleLogin(specialist); }));
                } else {
                    QMessageBox::warning(this, "Error", "Invalid rating. Please enter a number between 1 and 5.");
                }
            } else {
                QMessageBox::warning(this, "Error", "The specified client is not a user.");
            }
        } else {
            QMessageBox::warning(this, "Error", "User not found.");
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



        auto it = std::find_if(people.begin(), people.end(), [&](const Person* p) {
            return QString::fromStdString(p->getFirstName()) == specialistFirstName &&
                   QString::fromStdString(p->getLastName()) == specialistLastName;
        });

        if (it != people.end()) {
            Specialist* specialist = dynamic_cast<Specialist*>(*it);
            if (specialist) {
                bool ok;
                int rating = ratingEdit->text().toInt(&ok);
                if (ok && rating >= 1 && rating <= 5) {
                    specialist->addRating(rating);
                    specialist->addReview(reviewEdit->text().toStdString());
                    saveToDatabase();
                    clearLayout(mainLayout);
                    outputLabel->setText("Rating and review submitted successfully!");
                    mainLayout->addWidget(createButton("Back", [this, user]() { handleLogin(user); }));
                } else {
                    QMessageBox::warning(this, "Error", "Invalid rating. Please enter a number between 1 and 5.");
                }
            } else {
                QMessageBox::warning(this, "Error", "The specified client is not a specialist.");
            }
        } else {
            QMessageBox::warning(this, "Error", "Specialist not found.");
        }
    };

    mainLayout->addWidget(createButton("Submit", submitReview));
    mainLayout->addWidget(createButton("Back", [this, user]() { handleLogin(user); }));
}

void MainWindow::deleteUserByName(Admin* admin) {
    clearLayout(mainLayout);
    outputLabel->setText("Enter the first and last name of the user you want to delete:");
    outputLabel->setText("Enter client's first name:");
    QSpacerItem* spacer = new QSpacerItem(100, 100, QSizePolicy::Minimum, QSizePolicy::Fixed);
    QLineEdit* firstNameEdit = createLineEdit();
    mainLayout->addWidget(firstNameEdit);

    mainLayout->addWidget(new QLabel("Enter client's last name:", this));

    QLineEdit* lastNameEdit = createLineEdit();
    mainLayout->addWidget(lastNameEdit);

    QPushButton* deleteButton = createButton("Delete", [this, firstNameEdit, lastNameEdit, admin]() {
        QString firstName = firstNameEdit->text();
        QString lastName = lastNameEdit->text();

        auto it = std::find_if(people.begin(), people.end(), [&](const Person* p) {
            return QString::fromStdString(p->getFirstName()) == firstName &&
                   QString::fromStdString(p->getLastName()) == lastName;
        });

        if (it != people.end()) {
            delete *it;
            people.erase(it);
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

void MainWindow::logout() {
    mainMenu();
}