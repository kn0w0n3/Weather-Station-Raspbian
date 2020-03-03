#include "textdata.h"

TextData::TextData(QGraphicsItem *parent): QGraphicsTextItem(parent){

}

void TextData::textData(QString text){
    setPlainText(QString(text));
    setDefaultTextColor(Qt::white);
    setFont(QFont("times",20));
}

void TextData::greenTextData(QString text){
    setPlainText(QString(text));
    setDefaultTextColor(Qt::green);
    setFont(QFont("times",20));
}

void TextData::redTextData(QString text){
    setPlainText(QString(text));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times",20));
}

void TextData::blueTextData(QString  text){
    setPlainText(QString(text));
    setDefaultTextColor(QColor(9,146,217));
    setFont(QFont("times",35));
}

void TextData::yellowTextData(QString text){
    setPlainText(QString(text));
    setDefaultTextColor(QColor(245,237,16));
    setFont(QFont("times",35));
}
