#ifndef OUTDOORWEATHER_H
#define OUTDOORWEATHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class OutdoorWeather : public QObject
{
    Q_OBJECT
public:
    explicit OutdoorWeather(QObject *parent = nullptr);
    void makeRequest(QString);

signals:
    void dataReadyRead(QByteArray);

public slots:
    void readyRead(QNetworkReply *);

private:
    QNetworkAccessManager *qnam;

};

#endif // OUTDOORWEATHER_H
