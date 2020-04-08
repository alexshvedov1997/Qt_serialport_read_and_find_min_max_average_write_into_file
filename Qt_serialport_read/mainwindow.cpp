#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QVector>
#include <QString>
#include <algorithm>
#include <QDebug>
#include <numeric>
#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include <QThread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    serial->setPortName("COM5");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
   // serial->open(QIODevice::ReadWrite);
    vect =  new QVector<double>;
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialRecieve()));
    connect(ui->ButtonRes, SIGNAL(clicked()), this,SLOT(showResults()));
    connect(ui->startRead, SIGNAL(clicked()), this, SLOT(openPort()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::serialRecieve(){
    QThread thread;
    thread.msleep(10);
    QString str = serial->readLine();
   // QString str(serial->readAll());
    ui->label->setText("Открыт");
    ui->listWidget->addItem(str);
    double value;
    value = str.toDouble();
    vect->push_back(value);
}

void MainWindow::showResults(){
    serial->close();
    QString str = NULL;
    ui->label->setText("Закрыт");
    /*
    for(auto p = vect->begin(); p != vect->end(); p++){
        str = QString::number(*p);
    }
    */
    // QVector<double>::iterator max_val = std::max_element(vect->begin(), vect->end());
     ui->maxVal->setText(QString::number(maxValue()));
   //  QVector<double>::iterator min_val = std::min_element(vect->begin(), vect->end());
     ui->minVal->setText(QString::number(minValue()));
     ui->averageVal->setText(QString::number(averageValue()));
     writeInformation();
     writeSensorInformation();
}

double MainWindow::averageValue(){
    int count = vect->count();
    double sum_of_elem = std::accumulate(vect->begin() + 3 , vect->end(), 0);
    double res = sum_of_elem/(count -3 );
    return res;
}

void MainWindow::writeInformation(){
    QFile file("D:\\Qt_C++_Projects\\Qt_serialport_read\\Results.txt");
    if(file.open(QIODevice::Append | QIODevice::Text)){
        QTextStream writeStream(&file);
        writeStream<< QString(40, '-')<<"\n";
        writeStream<< "Results"<<"\n";
        writeStream<< "Average value: " << averageValue()<<"\n";
        writeStream<<"Min value: " << minValue() << "\n";
        writeStream<<"Max value: " << maxValue()<< "\n";
        file.close();
    }
}

double MainWindow::minValue(){
    auto min = std::min_element(vect->begin() + 3, vect->end());
    return *min;
}

double MainWindow::maxValue(){
    auto max = std::max_element(vect->begin() + 3, vect->end());
    return *max;
}

void MainWindow::writeSensorInformation(){
    QFile file("D:\\Qt_C++_Projects\\Qt_serialport_read\\all_data_from_sensor.txt");
    if(file.open(QIODevice::Append | QIODevice::Append)){
        QTextStream textStream(&file);
        textStream<<QString(40,'-')<<"\n";
        textStream<< "Start experiment"<< "\n";
        for(auto p = vect->begin() + 3; p != vect->end(); p++)
            textStream<<*p << "\n";
        textStream<< "End experiment"<< "\n";
        file.close();
    }
}

void MainWindow::openPort(){
    serial->open(QIODevice::ReadWrite);
}
















