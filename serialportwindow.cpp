#include "serialportwindow.h"
#include "ui_serialportwindow.h"
#include "settingsDefine.h"

#include <QDebug>
#include <QMessageBox>
SerialPortWindow::SerialPortWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialPortWindow)
{
    ui->setupUi(this);
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(!info.isBusy())
        {
            ui->comBox_Serial_U2270->addItem(info.portName());
            ui->comBox_Serial_Tester->addItem(info.portName());
        }
    }
    // Search in settings? if we allready have such names
    QSettings settings(SETTING_FILE,QSettings::IniFormat);
    int indx = ui->comBox_Serial_Tester->findText(settings.value(TESTER_PORT_SETTINGS).toString());
    if (indx != -1)
        ui->comBox_Serial_Tester->setCurrentIndex(indx);
    indx = ui->comBox_Serial_U2270->findText(settings.value(U2270_PORT_SETTINGS).toString());
    if (indx != -1)
        ui->comBox_Serial_U2270->setCurrentIndex(indx);
    deviceTester = new DeviceTester();
    deviceU = new DeviceU2270();

    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(accept()));
}
SerialPortWindow::SerialPortWindow(QWidget *parent, DeviceTester *devTester, DeviceU2270 *devU) :
    QDialog(parent),
    ui(new Ui::SerialPortWindow)
{
    ui->setupUi(this);
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(!info.isBusy())
        {
            ui->comBox_Serial_U2270->addItem(info.portName());
            ui->comBox_Serial_Tester->addItem(info.portName());
        }
    }
    // Search in settings? if we allready have such names
    QSettings settings(SETTING_FILE,QSettings::IniFormat);
    int indx = ui->comBox_Serial_Tester->findText(settings.value(TESTER_PORT_SETTINGS).toString());
    if (indx != -1)
        ui->comBox_Serial_Tester->setCurrentIndex(indx);
    indx = ui->comBox_Serial_U2270->findText(settings.value(U2270_PORT_SETTINGS).toString());
    if (indx != -1)
        ui->comBox_Serial_U2270->setCurrentIndex(indx);
    this->deviceTester = devTester;
    this->deviceU = devU;

    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(accept()));
}


SerialPortWindow::~SerialPortWindow()
{
    delete ui;
}

void SerialPortWindow::on_pBtn_Serial_Tester_Test_clicked()
{
    // check SafeTester on serial Port
    QPixmap mypix (":/img/img/CircleYellow.png");
    ui->label_4->setPixmap(mypix);
    ui->label_4->repaint();
    deviceTester->setPortName(ui->comBox_Serial_Tester->currentText());
    deviceTester->deviceOpenSerial();
    QByteArray parcell;
    bool a = deviceTester->deviceReadInfo(parcell);
    if (a)
    {
        qDebug() << "Safe tester GPT-79803 is connected";
        QSettings settings(SETTING_FILE,QSettings::IniFormat);
        settings.setValue(TESTER_PORT_SETTINGS, ui->comBox_Serial_Tester->currentText());
        QPixmap mypix (":/img/img/CircleGreen.png");
        ui->label_4->setPixmap(mypix);
    } else
    {
        qDebug() << "Safe tester GPT-79803 is not connected";
        QPixmap mypix (":/img/img/CircleRed.png");
        ui->label_4->setPixmap(mypix);
    }
    deviceTester->deviceCloseSerial();


}


void SerialPortWindow::on_pBtn_Serial_U2270_Test_clicked()
{
    // check SafeTester on serial Port
    QPixmap mypix (":/img/img/CircleYellow.png");
    ui->label_2->setPixmap(mypix);
    ui->label_2->repaint();
    deviceU->setPortName(ui->comBox_Serial_U2270->currentText());
    deviceU->deviceOpenSerial();
    QByteArray parcell;
    bool a = deviceU->deviceReadInfo(parcell);
    if (a)
    {
        qDebug() << "U2270 is connected";
        QSettings settings(SETTING_FILE,QSettings::IniFormat);
        settings.setValue(U2270_PORT_SETTINGS, ui->comBox_Serial_U2270->currentText());
        QPixmap mypix (":/img/img/CircleGreen.png");
        ui->label_2->setPixmap(mypix);
    } else
    {
        qDebug() << "U2270 is not connected";
        QPixmap mypix (":/img/img/CircleRed.png");
        ui->label_2->setPixmap(mypix);
    }
    deviceU->deviceCloseSerial();
}


void SerialPortWindow::on_pushButton_3_clicked()
{
    // Просто выходим
}


void SerialPortWindow::on_pushButton_4_clicked()
{
    // применить (подключиться)
    deviceTester->setPortName(ui->comBox_Serial_Tester->currentText());
    deviceTester->deviceOpenSerial();
    QByteArray parcell;
    bool a = deviceTester->deviceReadInfo(parcell);
    if (a)
    {
        qDebug() << "Safe tester GPT-79803 is connected";

        QSettings settings(SETTING_FILE,QSettings::IniFormat);
        settings.setValue(TESTER_PORT_SETTINGS, ui->comBox_Serial_Tester->currentText());

    } else
    {
        qDebug() << "Safe tester GPT-79803 is not connected";
    }
    deviceTester->deviceCloseSerial();
    deviceU->setPortName(ui->comBox_Serial_U2270->currentText());
    deviceU->deviceOpenSerial();
    QByteArray parcell_U;
    bool b = deviceU->deviceReadInfo(parcell_U);
    if (b)
    {
        qDebug() << "U2270 is connected";
        QSettings settings(SETTING_FILE,QSettings::IniFormat);
        settings.setValue(U2270_PORT_SETTINGS, ui->comBox_Serial_U2270->currentText());
    } else
    {
        qDebug() << "U2270 is not connected";
    }
    deviceU->deviceCloseSerial();
    if(!(a&&b))
    {
        QMessageBox::critical(this,"Ошибка подключения", "Не удалось подключиться к одному из устройств.\n1. GPT79803\n2. Установка У2270");
    }
}

