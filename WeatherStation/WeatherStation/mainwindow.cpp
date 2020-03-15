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
    serialBuffer = "";
    n1 = false;
    n2= false;
    n3 =false;
    iconActive = false;

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

      background = new BackgroundItem(QPixmap(":images/Weather_Gui2"));
      background->setPos(0,0);
      scene->addItem(background);

      lux = new BackgroundItem(QPixmap(":images/lux100"));
      lux->setPos(15,420);
      scene->addItem(lux);

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

      //Timer to request sensor data every 3 seconds
      requestSensData = new QTimer();
      connect(requestSensData, SIGNAL(timeout()), this, SLOT(requestSensorData()));
      requestSensData->start(3000);

      //Temperature in fahrenheight Display
      temperature = new TextData();
      temperature->setPos(690,120);
      scene->addItem(temperature); 

      //Temperature in celsius Display
      tempCelsius = new TextData();
      tempCelsius->setPos(690,190);
      scene->addItem(tempCelsius);

      //Humidity Display
      humidity = new TextData();
      humidity->setPos(690,260);
      scene->addItem(humidity);

      //Temperature in fahrenheight outdoor Display
      externalTempFahr = new TextData();
      externalTempFahr->setPos(50,120);
      scene->addItem(externalTempFahr);

      //Temperature in celsius outdoor Display
      externalTempCel = new TextData();
      externalTempCel->setPos(50,190);
      scene->addItem(externalTempCel);

      //Display external humidity data
      externalHumidity = new TextData();
      externalHumidity->setPos(50,260);
      scene->addItem(externalHumidity);

      //Display light sensor data
      lightData = new TextData();
      lightData->setPos(130,430);
      scene->addItem(lightData);

      //Display weather text from accuweather
      weatherText = new TextData();
      weatherText->setPos(390,320);
      scene->addItem(weatherText);

      //Get local weather data fron the accuweather API
      this->networkManager = new OutdoorWeather();
      connect(networkManager, SIGNAL(dataReadyRead(QString)), this, SLOT(processNetworkData(QString)));
      networkManager->makeRequest("http://dataservice.accuweather.com/currentconditions/v1/327147?apikey=d3PFMAGeiKnHepd7bVDZTGeqmWBaWfqt&details=true");
}

MainWindow::~MainWindow(){
    delete ui;
}

//Display the temperature sensor data on screen
void MainWindow::displayTempData(QString data){
    if(counter == 1){
        if(data.length() != 5){
            qDebug() << counter << " Error data";
        }
        if(data.length() == 5){
            tempCelsius->yellowTextData(data + " °C" );
            double celToFahrenheight = ((data.toDouble() * 9.0) / 5.0) + 32 ;
            QString temp = QString::number(celToFahrenheight, 'f', 2);
            temperature->yellowTextData(temp + " °F");
        }
    }    
}

//Display the humidity sensor data on screen
void MainWindow::displayHumidData(QString data){
    if(counter == 2){
        if(data.length() != 5){
        }
        if(data.length() == 5){
            humidity->yellowTextData(data + " %");
        }
    }
}

//Display the light sensor data on screen
void MainWindow::displayLightData(QString data){
    if(counter == 3){
        lightData->whiteTextData(data);
    }
}

//resize the window
void MainWindow::resizeEvent(QResizeEvent *event){
    QMainWindow::resizeEvent(event);
    ui->graphicsView->fitInView(0, 0, 901, 531, Qt::IgnoreAspectRatio);
}

//Set weather icon
void MainWindow::setWeatherIcon(int n){
    if(iconActive == true){
        delete icon;
        iconActive = false;
    }
    for(int i = 1; i <= 44; i++){
        if(n == i){
            QString iconNumber = QString::number(i);
            icon = new BackgroundItem(QPixmap(iconPath + iconNumber));
            icon->setPos(295,125);
            scene->addItem(icon);
            iconActive = true;
        }
    }
}

//Process the incoming serial data from arduino
void MainWindow::receiveSerialData(){
    int c = 0;
    serialData = serialPort->readAll();
    serialBuffer += QString::fromStdString(serialData.toStdString());
    QStringList tempList = serialBuffer.split("\n");

    foreach(QString value, tempList){
        c++;
        if(c == 1){
            if(value.length() == 5){
                n1 =true;
                counter++;
                displayTempData(value);
            }
        }
        if(c == 2){
            if(value.length() == 5){
                n2 = true;
                counter++;
                displayHumidData(value);
            }
        }
        if(c == 3){
            if(value.length() == 1 || value.length() == 2 || value.length() == 3 || value.length() == 4){
                n3 = true;
                counter++;
                displayLightData(value);
            }
        }
        if(c == 3){
            if( n1 == true && n2 == true && n3 == true){
                serialBuffer = "";
                n1 = false;
                n2 = false;
                n3 = false;
                c = 0;
                counter = 0;
            }
            else if(!(n1 == true && n2 == true && n3 == true)){
                serialBuffer = "";
                n1 = false;
                n2 = false;
                n3 = false;
                c =0;
                counter = 0;
            }
        }
    }
}

//Display the current time
void MainWindow::displayTime(){
    QTime cTime = QTime::currentTime();
    currentTime->textData(cTime.toString("hh:mm:ss"));
    QString vTime = cTime.toString("hh:mm:ss");
}

//Display the date
void MainWindow::showDate(){
    QDate cDate = QDate::currentDate();
    currentDate->textData(cDate.toString("MM/dd/yyyy"));
}

//Get current weather conditions from accuweather
void MainWindow::processNetworkData(QString data){
    QString replyText = data;
    QJsonDocument json_doc = QJsonDocument::fromJson(replyText.toUtf8());
    QJsonArray array = json_doc.array();

    QString humidity;
    QString tempFahr;
    QString tempCel;
    QString weatherTextE;
    double HumidDub;

    foreach(const QJsonValue &v, array){
        QJsonObject obj = v.toObject();

        HumidDub = obj.value("RelativeHumidity").toInt();
        humidity = QString::number(HumidDub,'f',2);
        externalHumidity->blueTextData(humidity + " %");

        tempFahr= QString::number(obj.value("Temperature").toObject().value("Imperial").toObject().value("Value").toDouble(),'f', 2);
        externalTempFahr->blueTextData(tempFahr + " °F");

        tempCel= QString::number(obj.value("Temperature").toObject().value("Metric").toObject().value("Value").toDouble(),'f', 2);
        externalTempCel->blueTextData(tempCel + " °C");

        weatherIcon = obj.value("WeatherIcon").toInt();
        setWeatherIcon(weatherIcon);

        weatherTextE = obj.value("WeatherText").toString();
        //weatherText->whiteTextDataMedium(weatherTextE);
    }
}

//Send signal to arduino to start and send data
void MainWindow::requestSensorData(){
    QString letter = "a";
    serialPort->write(letter.toStdString().c_str());
}
