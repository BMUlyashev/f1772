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
    bool isConnected();

    bool setFunction(QString function);
    int getFunction();//
    bool setRampTime(QString time);
    bool setTimer(QString time, QString typeSignal);

    QString getPortName();

private slots:

private:
    QSerialPort m_deviceTester;
    QByteArray m_response;
    QString m_portName;
    bool m_isConnected;

    const QString COMMAND_READ_INFO_TESTER = "*IDN?\r\n";
    const QString COMMAND_SET_FUNC_TESTER = QString("MANU:EDIT:MODE %1\r\n");
    const QString COMMAND_READ_FUNC_TESTER = QString("MANU:EDIT:MODE?\r\n");

    const QString COMMAND_SET_RTIME_TESTER = QString("MANU:RTIM %1\r\n");
    const QString COMMAND_READ_RTIME_TESTER = QString("MANU:RTIM?\r\n");

    const QString COMMAND_SET_TTIME_ACW_TESTER = QString("MANU:ACW:TTIM %1\r\n");
    const QString COMMAND_READ_TTIME_ACW_TESTER = QString("MANU:ACW:TTIM?\r\n");
    const QString COMMAND_SET_TTIME_DCW_TESTER = QString("MANU:DCW:TTIM %1\r\n");
    const QString COMMAND_READ_TTIME_DCW_TESTER = QString("MANU:DCW:TTIM?\r\n");

    const QString COMMAND_SET_FREQ_TESTER = QString("MANU:EDIT:MODE?\r\n");
    QByteArray writeAndRead(QString command, int timeout);
    void writeCommand(QString command, int timeout);
};

#endif // DEVICETESTER_H
