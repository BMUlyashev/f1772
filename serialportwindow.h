#ifndef SERIALPORTWINDOW_H
#define SERIALPORTWINDOW_H

#include <QDialog>
#include "devicetester.h"
#include "deviceu2270.h"

#include <QSerialPortInfo>
#include <QSettings>

namespace Ui {
class SerialPortWindow;
}

class SerialPortWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SerialPortWindow(QWidget *parent = nullptr);
    ~SerialPortWindow();

private slots:
    void on_pBtn_Serial_Tester_Test_clicked();

    void on_pBtn_Serial_U2270_Test_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::SerialPortWindow *ui;
    DeviceTester *deviceTester;
    DeviceU2270 *deviceU;
};

#endif // SERIALPORTWINDOW_H
