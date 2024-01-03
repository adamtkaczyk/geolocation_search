#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <databse_client_interface.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    try {
        w.init();
        w.show();
        return a.exec();
    } catch (const DatabaseException& e) {
        QMessageBox msgBox;
        msgBox.setText("Error with database: " + QString{e.what()});
        msgBox.exec();
    }
}
