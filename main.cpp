#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <databse_client_interface.hpp>
#include <geolocation_configuration.hpp>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    try {

        GeoLocationConfiguration configuration;
        if (argc < 2) {
            throw std::runtime_error("Incorrent input argument: " + std::string{argv[0]} + " <configuration_file>");
        }
        auto configuration_file = argv[1];
        configuration.readFromIniFile(configuration_file);
        MainWindow main_window;
        main_window.init(configuration);
        main_window.show();
        return application.exec();
    } catch (const DatabaseException& e) {
        QMessageBox msgBox;
        msgBox.setText("Error with database: " + QString{e.what()});
        msgBox.exec();
    } catch (const std::exception& e) {
        QMessageBox msgBox;
        msgBox.setText("Error: " + QString{e.what()});
        msgBox.exec();
    }
}
