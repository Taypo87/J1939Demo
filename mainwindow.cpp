#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LuaScriptEngine.h"
#include <QMap>
#include <functional>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    serial->setPortName("COM4");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadOnly))
            connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
    else
        qDebug() << "Failed to open port";

    handlers = {
        {"FUEL:", [this](const QString& line){ handleFuel(line); }},
        {"RPM:",  [this](const QString& line){ handleRPM(line); }},
        {"TEMP:", [this](const QString& line){ handleTemp(line); }},
    };
}
void MainWindow::readSerialData() {
    buffer.append(serial->readAll());

    while (buffer.size() >= 8) {
        QByteArray frame = buffer.left(8);
        buffer.remove(0, 8);

        quint16 rpm = static_cast<quint8>(frame[0]) | (static_cast<quint8>(frame[1]) << 8);
        quint16 temp = static_cast<quint8>(frame[2]) | (static_cast<quint8>(frame[3]) << 8);
        quint8 oil = static_cast<quint8>(frame[4]);
        quint8 battery = static_cast<quint8>(frame[5]);
        quint8 fuel = static_cast<quint8>(frame[6]);
        quint8 status = static_cast<quint8>(frame[7]);

        ui->rpmLabel->setText(QString::number(rpm) + " RPM");
        ui->tempLabel->setText(QString::number(temp) + " °C");
        ui->fuelBar->setValue(fuel);
        ui->oilLabel->setText(QString::number(oil) + " psi");
        ui->batteryLabel->setText(QString::number(battery / 10.0, 'f', 1) + " V");


        std::string tempStatus = lua->callFunction("get_temp_status", temp);
        qDebug() << "Temp status:" << QString::fromStdString(tempStatus);

        if (status & (1 << 0)) {
            qDebug() << "High temperature warning!";
        }
        if (status & (1 << 1)) {
            qDebug() << "Low oil pressure warning!";
        }

        QString fuelColor = fuel < 15 ? "red" : fuel < 50 ? "orange" : "green";
        ui->fuelBar->setStyleSheet(QString(R"(
            QProgressBar {
                border: 2px solid #555;
                border-radius: 5px;
                background-color: #222;
            }
            QProgressBar::chunk {
                background-color: %1;
                margin: 1px;
            }
        )").arg(fuelColor));
    }
}


void MainWindow::handleFuel(const QString& line) {
    bool ok = false;
    int fuel = line.mid(5).toInt(&ok);
    if (!ok) return;

    ui->fuelBar->setValue(fuel);

    QString color = fuel < 15 ? "red" :
                        fuel < 50 ? "orange" : "green";

    ui->fuelBar->setStyleSheet(QString(R"(
        QProgressBar {
            border: 2px solid #555;
            border-radius: 5px;
            background-color: #222;
        }
        QProgressBar::chunk {
            background-color: %1;
            margin: 1px;
        }
    )").arg(color));
}

void MainWindow::handleRPM(const QString& line) {
    bool ok = false;
    int rpm = line.mid(4).toInt(&ok);
    if (ok) {
        ui->rpmLabel->setText(QString::number(rpm) + " RPM");
    }
}

void MainWindow::handleTemp(const QString& line) {
    bool ok = false;
    int temp = line.mid(5).toInt(&ok);
    if (ok) {
        ui->tempLabel->setText(QString::number(temp) + " °C");
        std::string tempStatus = lua->callFunction("get_temp_status", temp);
        qDebug() << "Temp status:" << QString::fromStdString(tempStatus);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
