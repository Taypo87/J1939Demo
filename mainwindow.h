#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void readSerialData();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    void initSerialPort();

    void updateRPMDisplay(int rpm);

};
#endif // MAINWINDOW_H
