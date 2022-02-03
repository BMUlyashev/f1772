#ifndef DEVICEU2270_H
#define DEVICEU2270_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class DeviceU2270 : public QObject
{
    Q_OBJECT
public:

    enum U2270_Error{
        OK = 0,
        WRONG_ANSWER,
        NOT_CONNECTED,
    };


    explicit DeviceU2270(QObject *parent = nullptr);
    void setPortName(QString serialPortName);

    bool deviceReadInfo(QByteArray &answer);
    bool deviceOpenSerial();
    void deviceCloseSerial();
    int clearOutput(int board);  // disable all output relays on board #
    int setChanelForSignal(int board, int chanel, QString typeSignal);
    QString getPortName();
    int getTypeBoard(int boardNumber);
    bool isConnected();
    int getNumBoards();
     QString errosString(int numError);
private:

private slots:

private:
    QSerialPort m_deviceU2270;
    QByteArray m_response;
    QString m_portName;
    bool m_isConnected;

    const QString COMMAND_READ_INFO_U2270 = "*IDN?\r\n";
    const QString COMMAND_CLEAR_U2270 = "CLR\r\n";
    const QString COMMAND_READ_BOARD = QString("%1 TYPE ?\r\n");
    const QString TYPE_BOARD_COM = "COM\r\n";
    const QString COMMAND_SET_U2270 = "SET";
    const QString COMMAND_EXAMPE_U2270 = QString("%1 SET %2 %3\r\n");
    QByteArray writeAndRead(QString command, int timeout);

};

#endif // DEVICEU2270_H
