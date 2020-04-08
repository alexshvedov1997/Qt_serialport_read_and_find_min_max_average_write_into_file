#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtSerialPort/QSerialPort>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QVector<double> *vect;
    double averageValue();
    double minValue();
    double maxValue();
    void writeInformation();
    void writeSensorInformation();

private slots:
    void serialRecieve();
    void showResults();
    void openPort();

};
#endif // MAINWINDOW_H
