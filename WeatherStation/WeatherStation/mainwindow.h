#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QSerialPort>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QString>
#include <QDate>
#include <QTime>
#include <QTimer>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include "textdata.h"
#include "backgrounditem.h"
#include "outdoorweather.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void displayTempData(QString);
    void displayHumidData(QString);

    void resizeEvent(QResizeEvent *event) override;

public slots:
    void receiveSerialData();
    void displayTime();
    void showDate();
    void processNetworkData(QByteArray);

private:
    Ui::MainWindow *ui;
    QRegularExpression *myRegEx;
    OutdoorWeather *networkManager;
    QGraphicsScene *scene;
    QSerialPort *serialPort;
    BackgroundItem *background;
    BackgroundItem *cloudSun;

    TextData *temperature;
    TextData *tempCelsius;
    TextData *humidity;

    TextData *outDoorTempFar;
    TextData *outDoorTempCel;

    int counter;
    QTimer *refreshClock;
    QTimer *refreshDate;
    TextData *currentTime;
    TextData *currentDate;

};

#endif // MAINWINDOW_H
