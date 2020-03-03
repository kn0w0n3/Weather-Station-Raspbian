#include "outdoorweather.h"

OutdoorWeather::OutdoorWeather(QObject *parent) : QObject(parent){
    this->qnam = new QNetworkAccessManager(this);
    QObject::connect(this->qnam, SIGNAL(finished(QNetworkReply*)),this, SLOT(readyRead(QNetworkReply*)));
}

void OutdoorWeather::makeRequest(QString endpointRequest){
    qnam->get(QNetworkRequest(QUrl(endpointRequest)));
}

void OutdoorWeather::readyRead(QNetworkReply *reply){
    QByteArray myData;
    myData = reply->readAll();
    emit this->dataReadyRead(myData);
}
