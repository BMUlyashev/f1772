#ifndef DEVICETESTER_H
#define DEVICETESTER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>


class DeviceTester : public QObject
{
    Q_OBJECT
public:

    enum Status{
      NO_ERROR = 0,
      WRONG_ANSWER,
      NOT_CONNECTED,
    };
    enum SignalFunction{
        ACW,
        DCW
    };

    struct Measure {
        QString typeFunction;
        QString status;
        QString voltageValue;
        QString currentValue;
        QString time;
        int progress;
    };
    Measure m_testValue;
    QString m_deviceName;
    QString m_deviceSerialNumber;

    //Q_DECLARE_METATYPE(Measure)
    explicit DeviceTester(QObject *parent = nullptr);
    void setPortName(QString serialPortName);

    bool deviceReadInfo(QByteArray &answer);
    bool deviceOpenSerial();
    void deviceCloseSerial();
    bool isConnected();
    int getSignalFunction();

    bool setFunction(QString function);
    int getFunction();//
    bool setRampTime(QString time);
    bool setTimer(QString time, QString typeSignal);
    int setFrequencyForACW(QString frequency);
    int setLowCurrent(QString currentValue, QString typeSignal);
    int setHiCurrent(QString currentValue, QString typeSignal);
    int setVoltage(QString voltageValue, QString typeSignal);
    int startTesting(bool enable);
    int readMeasure(Measure *tester);

    bool waitForReady(int timeout);

    QString getPortName();
public slots:
    void closeSerial();
    bool openSerial();
private slots:
        void handleReadyRead();
private:
    QSerialPort m_deviceTester;
    QByteArray m_response;
    QString m_portName;
    bool m_isConnected;

    SignalFunction m_function;

    const QString COMMAND_READ_INFO_TESTER = "*IDN?\r\n";
    const QString COMMAND_SET_FUNC_TESTER = QString("MANU:EDIT:MODE %1\r\n");
    const QString COMMAND_READ_FUNC_TESTER = QString("MANU:EDIT:MODE?\r\n");

    const QString COMMAND_SET_RTIME_TESTER = QString("MANU:RTIM %1\r\n");
    const QString COMMAND_READ_RTIME_TESTER = QString("MANU:RTIM?\r\n");

    const QString COMMAND_SET_FREQ_TESTER = QString("MANU:ACW:FREQuency %1\r\n");
    const QString COMMAND_READ_FREQ_TESTER = QString("MANU:ACW:FREQuency?\r\n");

    const QString COMMAND_SET_LCUR_ACW_TESTER = QString("MANU:ACW:CLOSet %1\r\n");
    const QString COMMAND_READ_LCUR_ACW_TESTER = QString("MANU:ACW:CLOSet?\r\n");
    const QString COMMAND_SET_LCUR_DCW_TESTER = QString("MANU:DCW:CLOSet %1\r\n");
    const QString COMMAND_READ_LCUR_DCW_TESTER = QString("MANU:DCW:CLOSet?\r\n");

    const QString COMMAND_SET_HCUR_ACW_TESTER = QString("MANU:ACW:CHISet %1\r\n");
    const QString COMMAND_READ_HCUR_ACW_TESTER = QString("MANU:ACW:CHISet?\r\n");
    const QString COMMAND_SET_HCUR_DCW_TESTER = QString("MANU:DCW:CHISet %1\r\n");
    const QString COMMAND_READ_HCUR_DCW_TESTER = QString("MANU:DCW:CHISet?\r\n");

    const QString COMMAND_SET_TTIME_ACW_TESTER = QString("MANU:ACW:TTIM %1\r\n");
    const QString COMMAND_READ_TTIME_ACW_TESTER = QString("MANU:ACW:TTIM?\r\n");
    const QString COMMAND_SET_TTIME_DCW_TESTER = QString("MANU:DCW:TTIM %1\r\n");
    const QString COMMAND_READ_TTIME_DCW_TESTER = QString("MANU:DCW:TTIM?\r\n");

    const QString COMMAND_SET_VOLT_ACW_TESTER = QString("MANU:ACW:VOLTage %1\r\n");
    const QString COMMAND_READ_VOLT_ACW_TESTER = QString("MANU:ACW:VOLTage?\r\n");
    const QString COMMAND_SET_VOLT_DCW_TESTER = QString("MANU:DCW:VOLTage %1\r\n");
    const QString COMMAND_READ_VOLT_DCW_TESTER = QString("MANU:DCW:VOLTage?\r\n");

    const QString COMMAND_READ_STAUS_TESTER = QString("MEASure?\r\n");

    const QString COMMAND_START_TESTER = QString("FUNCtion:TEST ON\r\n");
    const QString COMMAND_STOP_TESTER = QString("FUNCtion:TEST OFF\r\n");
    const QString COMMAND_READ_TESTER = QString("FUNCtion:TEST?\r\n");

    QByteArray writeAndRead(QString command, int timeout);
    void writeCommand(QString command, int timeout);
    void setSignalFunction(QString function);
    QByteArray m_readData;



};
Q_DECLARE_METATYPE(DeviceTester::Measure);
#endif // DEVICETESTER_H
