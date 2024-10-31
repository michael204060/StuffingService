
#include <QApplication>
#include <QWidget>
#include "../headers/MainWindow.h"
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Database db;
    db.initializeDatabase();
    MainWindow window(db);
    window.show();
    return app.exec();
}