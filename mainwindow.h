#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <geolocation_search.hpp>
#include <geolocation_configuration.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool init(const GeoLocationConfiguration& configuration);

private slots:
    void handleGetLocationButton();
    void handleDeleteLocationButton();
private:
    Ui::MainWindow *ui;
    std::unique_ptr<GeolocationSearch> geolocation_serach_;
};
#endif // MAINWINDOW_H
