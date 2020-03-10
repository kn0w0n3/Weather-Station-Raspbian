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
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include<QJsonValue>

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
    void displayLightData(QString);
    void resizeEvent(QResizeEvent *event) override;
    void setWeatherIcon(int);


public slots:
    void receiveSerialData();
    void displayTime();
    void showDate();
    void processNetworkData(QString);
    void requestSensorData();

private:
    Ui::MainWindow *ui;
    QRegularExpression *myRegEx;
    OutdoorWeather *networkManager;
    QGraphicsScene *scene;
    QSerialPort *serialPort;
    BackgroundItem *background;
    BackgroundItem *icon;
    BackgroundItem *lux;

    QString iconPath = ":images/";
    bool iconActive;

    TextData *temperature;
    TextData *tempCelsius;
    TextData *humidity;
    TextData *lightData;

    TextData *externalTempFahr;
    TextData *externalTempCel;
    TextData *externalHumidity;
    TextData *weatherText;

    int counter;
    QTimer *refreshClock;
    QTimer *refreshDate;
    QTimer *requestSensData;
    TextData *currentTime;
    TextData *currentDate;
    QByteArray serialData;
    QString serialBuffer;
    bool n1;
    bool n2;
    bool n3;
    int weatherIcon;
};

#endif // MAINWINDOW_H
