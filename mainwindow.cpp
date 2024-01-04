#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <geolocation_search.hpp>
#include <geolocation_search_factory.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , geolocation_serach_(nullptr)
{
    ui->setupUi(this);

}

bool MainWindow::init(const GeoLocationConfiguration& configuration)
{
    //TODO: chage to read from config file or input paramiter
/*
    GeoLocationConfiguration config;
    config.database_type = GeoLocationConfiguration::DatabaseType::POSTGRESQL;
    config.db_name = "geolocations";
    config.db_user = "postgres";
    config.db_password = "testpassword123";
    config.db_hostname = "127.0.0.1";
    config.db_port = 5432;
    configuration.geolocation_client_type = GeoLocationConfiguration::GeoLocationClientType::IPSTACK;
    configuration.geolocation_client_url = "http://api.ipstack.com";
    configuration.api_key = "2361f872909e692d1a67ae3e538f4582";
*/

    geolocation_serach_ = GeoLocationFactory::create(configuration);

    connect(ui->get_location_button, &QPushButton::released, this, &MainWindow::handleGetLocationButton);
    connect(ui->delete_location_button, &QPushButton::released, this, &MainWindow::handleDeleteLocationButton);
    return true;
}

void MainWindow::handleGetLocationButton()
{
    auto host = ui->input_value_edit->text();
    if (host.isEmpty()) {
        return;
    }

    try {
        auto location = geolocation_serach_->getIPGeoLocation(host.toStdString());
        if (location) {
            ui->ip_edit->setText(QString::fromStdString(location->ip));
            ui->type_edit->setText(QString::fromStdString(location->type));
            ui->continent_code_edit->setText(QString::fromStdString(location->continent_code));
            ui->continent_name_edit->setText(QString::fromStdString(location->continent_name));
            ui->country_code_edit->setText(QString::fromStdString(location->country_code));
            ui->country_name_edit->setText(QString::fromStdString(location->country_name));
            ui->region_code_edit->setText(QString::fromStdString(location->region_code));
            ui->region_name_edit->setText(QString::fromStdString(location->region_name));
            ui->city_edit->setText(QString::fromStdString(location->city));
            ui->zip_edit->setText(QString::fromStdString(location->zip));
            ui->lonitude_edit->setText(QString::number(location->longitude));
            ui->latitude_edit->setText(QString::number(location->latitude));
        }
    } catch (const std::exception& e) {
        QMessageBox msgBox;
        msgBox.setText("Cannot get location value: " + QString{e.what()});
        msgBox.exec();
    }
}

void MainWindow::handleDeleteLocationButton()
{
    auto host = ui->input_value_edit->text();
    if (host.isEmpty()) {
        return;
    }

    try {
        if (geolocation_serach_->deleteLocation(host.toStdString())) {
            ui->ip_edit->clear();
            ui->type_edit->clear();
            ui->continent_code_edit->clear();
            ui->continent_name_edit->clear();
            ui->country_code_edit->clear();
            ui->country_name_edit->clear();
            ui->region_code_edit->clear();
            ui->region_name_edit->clear();
            ui->city_edit->clear();
            ui->zip_edit->clear();
            ui->lonitude_edit->clear();
            ui->latitude_edit->clear();
        }
    } catch (const std::exception& e) {
        QMessageBox msgBox;
        msgBox.setText("Cannot delete location: " + QString{e.what()});
        msgBox.exec();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
