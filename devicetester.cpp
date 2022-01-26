#include "devicetester.h"
#include <QThread>
#include <QDebug>
//#include <stdio.h>

DeviceTester::DeviceTester(QObject *parent) : QObject(parent)
{

    m_deviceTester.setBaudRate(9600);
    m_isConnected = false;
    m_function = ACW;
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

void DeviceTester::writeCommand(QString command, int timeout)
{
    if(m_isConnected){
        qDebug() <<  "Device Tester -> " << command;
        m_deviceTester.write(command.toUtf8());
        m_deviceTester.waitForBytesWritten(50);
        this->thread()->msleep(timeout);
    }
}

bool DeviceTester::setFunction(QString function)
{
    if(m_isConnected){
        setSignalFunction(function);
        // write command
        //qDebug() <<  "Device Tester -> " << COMMAND_SET_FUNC_TESTER.arg(function);
        writeCommand(COMMAND_SET_FUNC_TESTER.arg(function), 10);
        // check to enable the command
        qDebug() <<  "Device Tester -> " << COMMAND_READ_FUNC_TESTER;
        QByteArray answer = writeAndRead(COMMAND_READ_FUNC_TESTER, 200);
        qDebug() << QString(answer);
        if (QString(answer) == (function + "\r\n")){
            qDebug() << "Success.";
            return true;
        } else
            qDebug() << "Fail.";
            return false;
    } else
    {
        qDebug() << "Not connected";
        return false;
    }
}

bool DeviceTester::setRampTime(QString time)
{
    if(m_isConnected){
        // write command
        //qDebug() <<  "Device Tester -> " << COMMAND_SET_FUNC_TESTER.arg(function);
        writeCommand(COMMAND_SET_RTIME_TESTER.arg(time), 10);
        // check to enable the command
        qDebug() <<  "Device Tester -> " << COMMAND_READ_RTIME_TESTER;
        QByteArray answer = writeAndRead(COMMAND_READ_RTIME_TESTER, 100);
        qDebug() << QString(answer);    // answer chould be 000.0 S, but time is 10.2
        QString formatAnswer = QString("%1").arg(time.toDouble(), 5, 'f', 1, '0');
        if (QString(answer) == (formatAnswer + " S\r\n")){
            qDebug() << "Success.";
            return true;
        } else
            qDebug() << "Fail.";
            return false;
    } else
    {
        qDebug() << "Not connected";
        return false;
    }
}

bool DeviceTester::setTimer(QString time, QString typeSignal)
{
    if(m_isConnected){
        // write command
        //qDebug() <<  "Device Tester -> " << COMMAND_SET_FUNC_TESTER.arg(function);
        QByteArray answer;
        if (typeSignal == "DCW"){
            writeCommand(COMMAND_SET_TTIME_DCW_TESTER.arg(time), 10);
            qDebug() <<  "Device Tester -> " << COMMAND_READ_TTIME_DCW_TESTER;
            answer = writeAndRead(COMMAND_READ_TTIME_DCW_TESTER, 100);
        }
        else {
            writeCommand(COMMAND_SET_TTIME_ACW_TESTER.arg(time), 10);
            qDebug() <<  "Device Tester -> " << COMMAND_READ_TTIME_ACW_TESTER;
            answer = writeAndRead(COMMAND_READ_TTIME_ACW_TESTER, 100);
        }// check to enable the command
        qDebug() << QString(answer);    // answer chould be 000.0 S, but time is 10.2
        QString formatAnswer = QString("%1").arg(time.toDouble(), 5, 'f', 1, '0');
        if (QString(answer) == (formatAnswer + " S\r\n")){
            qDebug() << "Success.";
            return true;
        } else {
            qDebug() << "Fail.";
            return false;
        }
    } else {
        qDebug() << "Not connected";
        return false;
    }
}
void DeviceTester::setSignalFunction(QString function)
{
    if(function == "DCW")
        m_function = DCW;
    else
        m_function = ACW;
}
int DeviceTester::getSignalFunction()
{
    return m_function;
}

int DeviceTester::setFrequencyForACW(QString frequency)
{
    int status = NO_ERROR;
    if (m_isConnected){
        QByteArray answer;
        writeCommand(COMMAND_SET_FREQ_TESTER.arg(frequency), 10);
        qDebug() <<  "Device Tester -> " << COMMAND_READ_FREQ_TESTER;
        answer = writeAndRead(COMMAND_READ_FREQ_TESTER, 100);
        qDebug() << QString(answer);
        if (QString(answer) == (QString("%1 Hz\r\n").arg(frequency))){
            qDebug() << "Success!";
            status = NO_ERROR;
        } else {
            qDebug() << "No success!";
            status = WRONG_ANSWER;
        }

    } else {
        qDebug() << "Not connected";
        status = NOT_CONNECTED;
    }
    return status;
}
int DeviceTester::setLowCurrent(QString currentValue, QString typeSignal)
{
    int status;
    if(m_isConnected){
        // write command
        //qDebug() <<  "Device Tester -> " << COMMAND_SET_FUNC_TESTER.arg(function);

        QByteArray answer;
        if (typeSignal == "DCW"){
            writeCommand(COMMAND_SET_LCUR_DCW_TESTER.arg(currentValue), 10);
            qDebug() <<  "Device Tester -> " << COMMAND_READ_LCUR_DCW_TESTER;
            answer = writeAndRead(COMMAND_READ_LCUR_DCW_TESTER, 100);
        }
        else {
            writeCommand(COMMAND_SET_LCUR_ACW_TESTER.arg(currentValue), 10);
            qDebug() <<  "Device Tester -> " << COMMAND_READ_LCUR_ACW_TESTER;
            answer = writeAndRead(COMMAND_READ_LCUR_ACW_TESTER, 100);
        }// check to enable the command
        qDebug() << QString(answer);    // answer chould be 0.000mA, but time is 10.2
        QStringList value = QString(answer).split('m');
        double mValue = 0.0;
        if (value[0] != NULL)
            mValue = value[0].toDouble();
        if (mValue == currentValue.toDouble()){
            qDebug() << "Success.";
            status = NO_ERROR;
        } else {
            qDebug() << "Fail.";
            status = WRONG_ANSWER;
        }
    } else {
        qDebug() << "Not connected";
        status = NOT_CONNECTED;
    }
    return status;
}

int DeviceTester::setHiCurrent(QString currentValue, QString typeSignal)
{
    int status;
    if(m_isConnected){
        // write command
        //qDebug() <<  "Device Tester -> " << COMMAND_SET_FUNC_TESTER.arg(function);

        QByteArray answer;
        if (typeSignal == "DCW"){
            writeCommand(COMMAND_SET_HCUR_DCW_TESTER.arg(currentValue), 10);
            qDebug() <<  "Device Tester -> " << COMMAND_READ_HCUR_DCW_TESTER;
            answer = writeAndRead(COMMAND_READ_HCUR_DCW_TESTER, 150);
        }
        else {
            writeCommand(COMMAND_SET_HCUR_ACW_TESTER.arg(currentValue), 10);
            qDebug() <<  "Device Tester -> " << COMMAND_READ_HCUR_ACW_TESTER;
            answer = writeAndRead(COMMAND_READ_HCUR_ACW_TESTER, 150);
        }// check to enable the command
        qDebug() << QString(answer);    // answer chould be 0.000mA, but time is 10.2
        QStringList value = QString(answer).split('m');
        double mValue = 0.0;
        if (value[0] != NULL)
            mValue = value[0].toDouble();
        if (mValue == currentValue.toDouble()){
            qDebug() << "Success.";
            status = NO_ERROR;
        } else {
            qDebug() << "Fail.";
            status = WRONG_ANSWER;
        }
    } else {
        qDebug() << "Not connected";
        status = NOT_CONNECTED;
    }
    return status;
}

int DeviceTester::setVoltage(QString voltageValue, QString typeSignal)
{
    int status;
    if(m_isConnected){
        // write command
        //qDebug() <<  "Device Tester -> " << COMMAND_SET_FUNC_TESTER.arg(function);

        QByteArray answer;
        if (typeSignal == "DCW"){
            writeCommand(COMMAND_SET_VOLT_DCW_TESTER.arg(voltageValue), 10);
            qDebug() <<  "Device Tester -> " << COMMAND_READ_VOLT_DCW_TESTER;
            answer = writeAndRead(COMMAND_READ_VOLT_DCW_TESTER, 150);
        }
        else {
            writeCommand(COMMAND_SET_VOLT_ACW_TESTER.arg(voltageValue), 10);
            qDebug() <<  "Device Tester -> " << COMMAND_READ_VOLT_ACW_TESTER;
            answer = writeAndRead(COMMAND_READ_VOLT_ACW_TESTER, 150);
        }// check to enable the command
        qDebug() << QString(answer);    // answer chould be 0.000mA, but time is 10.2
        QStringList value = QString(answer).split('k');
        double mValue = 0.0;
        if (value[0] != NULL)
            mValue = value[0].toDouble();
        if (mValue == voltageValue.toDouble()){
            qDebug() << "Success.";
            status = NO_ERROR;
        } else {
            qDebug() << "Fail.";
            status = WRONG_ANSWER;
        }
    } else {
        qDebug() << "Not connected";
        status = NOT_CONNECTED;
    }
    return status;
}
