#ifndef DEVICETESTER_H
#define DEVICETESTER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>


class DeviceTester : public QObject
{
    Q_OBJECT
public:
    explicit DeviceTester(QObject *parent = nullptr);
    void setPortName(QString serialPortName);

    bool deviceReadInfo(QByteArray &answer);
    bool deviceOpenSerial();
    void deviceCloseSerial();
private slots:

private:
    QSerialPort m_deviceTester;
    QByteArray m_response;


    const QString COMMAND_READ_INFO_TESTER = "*IDN?\r\n";
    QByteArray writeAndRead(QString command, int timeout);
};

#endif // DEVICETESTER_H
