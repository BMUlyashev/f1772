#ifndef WORKER_H
#define WORKER_H

#include <QObject>

#include "devicetester.h"
#include "deviceu2270.h"
#include "tableviewconfigmodel.h"
#include <QTimer>
#include <QEventLoop>
#include <QWaitCondition>
#include <QMutex>
class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    Worker(QObject *parent, DeviceTester *devTester, DeviceU2270 *devU, TableViewConfigModel *model);
    bool m_stop;
public slots:
        void run();
        void slotTimerReadStatus(); // read information about status tester after start every XX ms
        void slotCheckEnd();
        void slotTestingFail();

signals:
    void statusPreparation(int);
    void finished();
    void measure(DeviceTester::Measure);
    void statusStepProgress(int, int);
    void statusFail();

    void closeSerial();
    void openSerial();
private slots:

private:
    void delay(int millisecondsWait);

    DeviceTester *devTester;
    DeviceU2270 *devU;
    TableViewConfigModel *model;
//    QTimer *m_timerStatus;
//    QTimer *m_timerCheck;
    double m_totalTime;    // time for progress on chanel
    QEventLoop loop;
    long totalTime;
    double leftTime;  //  Время которое осталось
};

#endif // WORKER_H
