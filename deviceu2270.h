#ifndef DEVICEU2270_H
#define DEVICEU2270_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class DeviceU2270 : public QObject
{
    Q_OBJECT
public:
    explicit DeviceU2270(QObject *parent = nullptr);
    void setPortName(QString serialPortName);

    bool deviceReadInfo(QByteArray &answer);
    bool deviceOpenSerial();
    void deviceCloseSerial();

    QString getPortName();

private slots:

private:
    QSerialPort m_deviceU2270;
    QByteArray m_response;

    bool m_isConnected;

    const QString COMMAND_READ_INFO_U2270 = "*IDN?\r\n";
    QByteArray writeAndRead(QString command, int timeout);

};

#endif // DEVICEU2270_H
