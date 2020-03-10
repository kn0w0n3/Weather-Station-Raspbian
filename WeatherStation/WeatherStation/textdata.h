#ifndef TEXTDATA_H
#define TEXTDATA_H
#include <QGraphicsTextItem>
#include <QFont>

class TextData: public QGraphicsTextItem{

public:
    TextData(QGraphicsItem * parent=nullptr);
    void textData(QString);
    void whiteTextData(QString);
     void whiteTextDataMedium(QString);
    void greenTextData(QString);
    void redTextData(QString);
    void blueTextData(QString);
    void yellowTextData(QString);

public slots:

private:

};
#endif // TEXTDATA_H
