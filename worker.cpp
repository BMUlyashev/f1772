#include "worker.h"
#include <QDebug>
#include <QThread>
#include <QEventLoop>
Worker::Worker(QObject *parent) : QObject(parent)
{


}

Worker::Worker(QObject *parent, DeviceTester *devTester, DeviceU2270 *devU, TableViewConfigModel *model)
            : QObject(parent)
{
    this->devTester = devTester;
    this->model = model;
    this->devU = devU;

}
void Worker::slotTimerReadStatus()
{
    // read status tester and emit signal
    qDebug() << "Read status from Tester";
    DeviceTester::Measure data;
    int status = devTester->readMeasure(&data);
    emit measure(data);
    if(data.status == "PASS")
    {

    }

}

void Worker::slotCheckEnd()
{
    qDebug() << "Stop read timer";
}

void Worker::delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

void Worker::run()
{

    int progress = (100 / model->rowCount());
    int plusBoard, minusBoard, plusChanel, minusChanel, plusChanelOnBoard, minusChanelOnBoard;
    int err;
    int countAttemts = 1;
    for (int i = 0; i < model->rowCount(); ++i){
        qDebug() <<"Step " << model->index(i, TableViewConfigModel::COLUMN_STEP).data().toString() << ": "
                 << "Plus=" << model->index(i, TableViewConfigModel::COLUMN_PLUS).data().toString() << " "
                 << "Minus=" << model->index(i, TableViewConfigModel::COLUMN_MINUS).data().toString() << " "
                 << "Func=" << model->index(i, TableViewConfigModel::COLUMN_FUNC).data().toString() << " "
                 << "Freq=" << model->index(i, TableViewConfigModel::COLUMN_FREQ).data().toString() << " "
                 << "Volt=" << model->index(i, TableViewConfigModel::COLUMN_VOLT).data().toString() << " "
                 << "LoCur=" << model->index(i, TableViewConfigModel::COLUMN_LCUR).data().toString() << " "
                 << "HiCur=" << model->index(i, TableViewConfigModel::COLUMN_HCUR).data().toString() << " "
                 << "Ramp=" << model->index(i, TableViewConfigModel::COLUMN_RAMP).data().toString() << " "
                 << "Time=" << model->index(i, TableViewConfigModel::COLUMN_TIME).data().toString() << " ";
        /* Main thread */
        emit statusPreparation(i);
        QTimer timerStatus;
        connect(&timerStatus, SIGNAL(timeout()), this, SLOT(slotTimerReadStatus()));
        QTimer timerWait;
        QEventLoop loop;
        connect(&timerWait, SIGNAL(timeout()), &loop, SLOT(quit()));
        connect(&timerWait, SIGNAL(timeout()), this, SLOT(slotCheckEnd()));
        connect(&timerWait, SIGNAL(timeout()), &timerStatus, SLOT(stop()));
        int waitTime = model->index(i, TableViewConfigModel::COLUMN_TIME).data().toDouble() + model->index(i, TableViewConfigModel::COLUMN_RAMP).data().toDouble();
        timerWait.start(waitTime*1000+1000); // waitTime in seconds, but argument should be in milliseconds
        timerStatus.start(500);
        loop.exec();

        //delay(5000);

    }
    emit finished();
}
