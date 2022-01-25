#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <QThread>
#include <QObject>
#include "devicetester.h"
#include "deviceu2270.h"
#include "tableviewconfigmodel.h"

class TestThread : public QThread
{
    Q_OBJECT
public:

    explicit TestThread(QObject *parent = nullptr);
    TestThread(QObject *parent, DeviceTester *devTester, TableViewConfigModel *model, DeviceU2270 *devU);
    void run();

signals:
    void stepChanged(int);
    void finished();
    void statusPreparation(int);
    void statusProgress(int);


public slots:

private:
//    const DeviceTester *devTester;
    DeviceTester *devTester;
    DeviceU2270 *devU;
    TableViewConfigModel *model;


};

#endif // TESTTHREAD_H
