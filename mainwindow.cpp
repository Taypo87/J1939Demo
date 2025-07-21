#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initSerialPort();
}



void MainWindow::initSerialPort(){

    serial = new QSerialPort(this);
    serial->setPortName("COM4");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
    serial->open(QIODevice::ReadOnly);

}
void MainWindow::readSerialData() {

    static QByteArray buffer;
    QByteArray message;
    quint16 rpm;

    buffer.append(serial->readAll());

    if (buffer.size() >= 64) {
        message = buffer.left(64);
        buffer.remove(0, 64);
        rpm = static_cast<quint8>(message[0] | static_cast<quint8>(message[1] << 8));
        ui->label_rpmgauge->setText(QString::number(rpm) + "RPM");
    }
}
void updateRPMDisplay(int rpm) {

}



MainWindow::~MainWindow()
{
    delete ui;
}

