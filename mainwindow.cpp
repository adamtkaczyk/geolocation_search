#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <geolocation_search.hpp>
#include <geolocation_search_factory.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , geolocation_serach_(GeoLocationFactory::create([](){
        //TODO: chage to read from config file or input paramiter
        GeoLocationConfiguration config;
        config.database_type = GeoLocationConfiguration::DatabaseType::POSTGRESQL;
        config.geolocation_client_type = GeoLocationConfiguration::GeoLocationClientType::IPSTACK;
        config.geolocation_client_url = "http://api.ipstack.com";
        config.api_key = "2361f872909e692d1a67ae3e538f4582";
        return config;
    }()))
{
    ui->setupUi(this);
    connect(ui->get_location_button, &QPushButton::released, this, &MainWindow::handleButton);
}

void MainWindow::handleButton()
{
    auto host = ui->input_value_edit->text();
    try {
        auto location = geolocation_serach_->getIPGeoLocation(host.toStdString());
        if (location) {
            ui->lonitude_edit->setText(QString::number(location->longitude));
            ui->latitude_edit->setText(QString::number(location->latitude));
        }
    } catch (std::exception& e) {
        QMessageBox msgBox;
        msgBox.setText("Cannot get location value");
        msgBox.exec();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
