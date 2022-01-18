#include "deviceu2270.h"
#include <QThread>
#include <QDebug>

DeviceU2270::DeviceU2270(QObject *parent) : QObject(parent)
{
    m_deviceU2270.setBaudRate(9600);
}
void DeviceU2270::setPortName(QString serialPortName)
{
    m_deviceU2270.setPortName(serialPortName);
}

QByteArray DeviceU2270::writeAndRead(QString command, int timeout)
{
    if(m_deviceU2270.isOpen())
    {
        m_deviceU2270.write(command.toUtf8());
        m_deviceU2270.waitForBytesWritten(50);
        this->thread()->msleep(timeout);
        m_deviceU2270.waitForReadyRead(50);
        return m_deviceU2270.readAll();
    } else {
        return QByteArray("");
    }
}
bool DeviceU2270::deviceReadInfo(QByteArray &answer)
{
    answer = writeAndRead(COMMAND_READ_INFO_U2270, 200);
    qDebug() <<  " -> " << answer;
    QStringList data = QString(answer).split(',');
    if (data[0] == "OMA")
        return true;
    else
        return false;
}
bool DeviceU2270::deviceOpenSerial()
{
    return m_deviceU2270.open(QIODevice::ReadWrite);
}
void DeviceU2270::deviceCloseSerial()
{
    if(m_deviceU2270.isOpen())
        m_deviceU2270.close();
}
