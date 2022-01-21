#include "devicetester.h"
#include <QThread>
#include <QDebug>
DeviceTester::DeviceTester(QObject *parent) : QObject(parent)
{

    m_deviceTester.setBaudRate(9600);
}

void DeviceTester::setPortName(QString serialPortName)
{
    m_deviceTester.setPortName(serialPortName);
    m_portName = m_deviceTester.portName();
}

QByteArray DeviceTester::writeAndRead(QString command, int timeout)
{
    if(m_deviceTester.isOpen())
    {
        m_deviceTester.write(command.toUtf8());
        m_deviceTester.waitForBytesWritten(50);
        this->thread()->msleep(timeout);
        m_deviceTester.waitForReadyRead(50);
        return m_deviceTester.readAll();
    } else {
        return QByteArray("");
    }
}

bool DeviceTester::deviceReadInfo(QByteArray &answer)
{
    answer = writeAndRead(COMMAND_READ_INFO_TESTER, 200);
    qDebug() <<  " -> " << answer;
    QStringList data = QString(answer).split(',');
    if (data[0] == "GPT-79803")
        m_isConnected = true;
        else
        m_isConnected = false;
    return  m_isConnected;

}

bool DeviceTester::deviceOpenSerial()
{
    return m_deviceTester.open(QIODevice::ReadWrite);
}
void DeviceTester::deviceCloseSerial()
{
    if(m_deviceTester.isOpen())
        m_deviceTester.close();
}

bool DeviceTester::isConnected()
{
    return m_isConnected;
}
QString DeviceTester::getPortName()
{
    return m_portName;
}
