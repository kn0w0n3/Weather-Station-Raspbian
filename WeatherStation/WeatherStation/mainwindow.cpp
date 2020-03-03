#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    setStyleSheet("background-color: transparent;");
    ui->graphicsView->setStyleSheet("background: transparent");
    ui->menuBar->setStyleSheet("QMenuBar{background-image: url(:/images/menubar.png); border: 0px solid;}"
                                   "QMenuBar::item {background-color: transparent; color: white}"
                                   "QMenuBar::item:selected {background-color: #2f2f2f; color: white}"
                                   "QMenu{background-color: #2f2f2f; color: white}"
                                   "QMenu::item:selected{background: #ffffff; color: black}");
    counter = 0;

    this->myRegEx = new QRegularExpression("Imperial\\\":{\\\"Value\\\":\\d\\d.\\d");
    this->networkManager = new OutdoorWeather();
    connect(networkManager, SIGNAL(dataReadyRead(QByteArray)), this, SLOT(processNetworkData(QByteArray)));
    networkManager->makeRequest("http://dataservice.accuweather.com/currentconditions/v1/327147?apikey=d3PFMAGeiKnHepd7bVDZTGeqmWBaWfqt&details=true");

    //Check for connected serial devices
      foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
          qDebug()<<"Name: "<<info.portName();
          qDebug()<<"Description: "<<info.description();
          qDebug()<<"Manufactures: "<<info.manufacturer();
      }

      //Setup the serial port communication
      serialPort = new QSerialPort();
      serialPort->setPortName("/dev/ttyACM0");
      serialPort->setBaudRate(QSerialPort::Baud115200);
      serialPort->setDataBits(QSerialPort::Data8);
      serialPort->setParity(QSerialPort::NoParity);
      serialPort->setStopBits(QSerialPort::OneStop);
      serialPort->setFlowControl(QSerialPort::NoFlowControl);
      serialPort -> open(QIODevice::ReadWrite);
      connect(serialPort,SIGNAL(readyRead()),this,SLOT(receiveSerialData()));

      //Create a graphics scene and pass it to the view
      scene = new QGraphicsScene();
      scene->setSceneRect(0,0,901,531);

      ui->graphicsView->setScene(scene);
      ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      setCentralWidget(ui->graphicsView);

      background = new BackgroundItem(QPixmap(":images/Weather_Gui"));
      background->setPos(0,0);
      scene->addItem(background);

      cloudSun= new BackgroundItem(QPixmap(":images/cs300"));
      cloudSun->setPos(295,125);
      scene->addItem(cloudSun);

      //Date Text
      currentDate = new TextData();
      currentDate->setPos(703,5);
      scene->addItem(currentDate);

      //Clock Text
      currentTime = new TextData();
      currentTime->setPos(403,5);
      scene->addItem(currentTime);

      //Timer to update the clock every 1 second
      refreshClock = new QTimer();
      connect(refreshClock, SIGNAL(timeout()), this, SLOT(displayTime()));
      refreshClock->start(1000);

      //Timer to refresh the date every 1 second
      refreshDate = new QTimer();
      connect(refreshDate, SIGNAL(timeout()), this, SLOT(showDate()));
      refreshDate->start(1000);

      //Temperature in farenheight Display
      temperature = new TextData();
      temperature->setPos(700,120);
      scene->addItem(temperature); 

      //Temperature in celsius Display
      tempCelsius =new TextData();
      tempCelsius->setPos(700,190);
      scene->addItem(tempCelsius);

      //Humidity Display
      humidity = new TextData();
      humidity->setPos(700,260);
      scene->addItem(humidity);

      //Temperature in farenheight outdoor Display
      outDoorTempFar = new TextData();
      outDoorTempFar->setPos(55,120);
      scene->addItem(outDoorTempFar);


      //Temperature in celsius outdoor Display
      outDoorTempCel = new TextData();
      outDoorTempCel->setPos(55,190);
      scene->addItem(outDoorTempCel);

}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::displayTempData(QString data){
    if(counter == 1){
        if(data.length() != 5){
            qDebug() << counter << " Error data";
        }
        if(data.length() == 5){
            tempCelsius->yellowTextData(data + " °C" );
            double celToFarenheight = ((data.toDouble() * 9.0) / 5.0) + 32;
            QString temp = QString::number(celToFarenheight);
            temperature->yellowTextData(temp + " °F");
        }
    }    
}

void MainWindow::displayHumidData(QString data){
    if(counter == 2){
        if(data.length() != 5){
        }
        if(data.length() == 5){
            humidity->yellowTextData(data + " %");
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event){
    QMainWindow::resizeEvent(event);
    ui->graphicsView->fitInView(0, 0, 901, 531, Qt::IgnoreAspectRatio);
}

void MainWindow::receiveSerialData(){
    counter++;
    QByteArray bytearray;
    bytearray = serialPort->readLine().trimmed();
    QString receivedData = QString(bytearray);

    qDebug() << receivedData;

    if(counter == 1){
        displayTempData(receivedData);
    }
    else if(counter == 2){
        displayHumidData(receivedData);
        counter = 0;
    }
}

void MainWindow::displayTime(){
    QTime cTime = QTime::currentTime();
    currentTime->textData(cTime.toString("hh:mm:ss"));
    QString vTime = cTime.toString("hh:mm:ss");
}

void MainWindow::showDate(){
    QDate cDate = QDate::currentDate();
    currentDate->textData(cDate.toString("MM/dd/yyyy"));
}

void MainWindow::processNetworkData(QByteArray data){
    qDebug() << data;
    QString temperature = "";
    QRegularExpressionMatch myRegExMatch = this->myRegEx->match(data);
    temperature = myRegExMatch.captured(0);
    this->myRegEx->setPattern("\\d\\d.\\d");
    myRegExMatch = myRegEx->match(temperature);
    temperature = myRegExMatch.captured(0);
    qDebug() << temperature <<"Trying";
    outDoorTempFar->blueTextData(temperature + " °F");

    double farToCel = (5.0 / 9.0) * (temperature.toDouble() - 32.0);
    QString temp = QString::number(farToCel, 'f', 2);
    outDoorTempCel->blueTextData( temp + " °C");
    //(fahrenheit - 32.0) * 5.0 / 9.0;
}
