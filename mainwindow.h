#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include "LuaScriptEngine.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

QMap<QString, std::function<void(const QString&)>> handlers;
void handleFuel(const QString& line);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void handleFuel(const QString& line);
    void handleRPM(const QString& line);
    void handleTemp(const QString& line);


private slots:
    void readSerialData();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QByteArray buffer;
    LuaScriptEngine *lua;
};
#endif // MAINWINDOW_H
