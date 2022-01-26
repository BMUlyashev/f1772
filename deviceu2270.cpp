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
    m_portName = m_deviceU2270.portName();
}

QByteArray DeviceU2270::writeAndRead(QString command, int timeout)
{
    if(m_deviceU2270.isOpen())
    {
        qDebug() << "Send request << " << command;
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
    answer = writeAndRead(COMMAND_READ_INFO_U2270, 100);
    qDebug() <<  " -> " << answer;
    QStringList data = QString(answer).split(',');
    if (data[0] == "OMA")
        m_isConnected = true;
    else
        m_isConnected = false;
    return m_isConnected;
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

bool DeviceU2270::isConnected()
{
    return m_isConnected;
}
QString DeviceU2270::getPortName()
{
    return m_portName;
}
QString DeviceU2270::errosString(int numError)
{
    switch (numError) {
        case U2270_Error::OK:
            return "OK";
            break;
    case U2270_Error::WRONG_ANSWER:
        return "Wrong answer";
        break;
    case U2270_Error::NOT_CONNECTED:
        return "Not Connected";
        break;
    default:
        return "Amknown Error";
    }
}

int DeviceU2270::clearOutput(int board)
{
    if(m_isConnected)
    {
        QByteArray answer = writeAndRead(QString("%1 ").arg(board) + COMMAND_CLEAR_U2270, 100);
        if (QString(answer) == "OK\r\n")
            return U2270_Error::OK;
        else
            return U2270_Error::WRONG_ANSWER;
    } else
    {
        return U2270_Error::NOT_CONNECTED;
    }
}

int DeviceU2270::setChanelForSignal(int board, int chanel, QString typeSignal)
{
    if(m_isConnected)
    {
         QByteArray answer = writeAndRead(COMMAND_EXAMPE_U2270.arg(board).arg(chanel).arg(typeSignal), 100);
         if (QString(answer) == "OK\r\n")
             return U2270_Error::OK;
         else
             return U2270_Error::WRONG_ANSWER;
    } else
    {
        return U2270_Error::NOT_CONNECTED;
    }
}
