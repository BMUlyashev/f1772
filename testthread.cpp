#include "testthread.h"
#include <QDebug>

TestThread::TestThread(QObject *parent) : QThread(parent)
{

}

TestThread::TestThread(QObject *parent, DeviceTester *devTester, TableViewConfigModel *model, DeviceU2270 *devU) : QThread(parent)
{
    this->devTester = devTester;
    this->model = model;
    this->devU = devU;

    loop = new QEventLoop;
}

TestThread::~TestThread()
{
    delete timerReadInfo;
    delete timerCheck;
    delete loop;
}
void TestThread::run()
{
    timerReadInfo = new QTimer(this);
    connect(timerReadInfo, SIGNAL(timeout()), this, SLOT(slotTimerReadStatus()));
    timerCheck = new QTimer(this);
    timerCheck->setSingleShot(true);
    connect(timerCheck, SIGNAL(timeout()), this, SLOT(slotCheckEnd()));
    int progress = (100 / model->rowCount());
    int plusBoard, minusBoard, plusChanel, minusChanel, plusChanelOnBoard, minusChanelOnBoard;
    int err;
    int countAttemts = 1;


    for (int i = 0; i < model->rowCount(); ++i)
    {

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

        /* Set U2270 for signals*/

        plusChanel = model->index(i, TableViewConfigModel::COLUMN_PLUS).data().toInt();
        plusBoard = (plusChanel - 1)/10 + 1;
        plusChanelOnBoard = !(plusChanel % 10) ? 10 : (plusChanel % 10);
        qDebug() << QString("Clear (plus) board %1. Attemp #").arg(plusBoard).arg(countAttemts);
        err = devU->clearOutput(plusBoard);   // ToDo add if have not answer
            // check for answer errors
        qDebug() << "Status: " << devU->errosString(err);

        minusChanel = model->index(i, TableViewConfigModel::COLUMN_MINUS).data().toInt();
        minusBoard = (minusChanel - 1)/10 + 1;
        minusChanelOnBoard = !(minusChanel % 10) ? 10 : (minusChanel % 10);
        qDebug() << QString("Clear (minus) board %1. Attemp #").arg(plusBoard).arg(countAttemts);
        err = devU->clearOutput(minusBoard);   // ToDo add if have not answer
            // check for answer errors
        qDebug() << "Status: " << devU->errosString(err);

        /* set U2270 for sihnals*/
        err = devU->setChanelForSignal(plusBoard, plusChanelOnBoard, "P");
        qDebug() << "Status: " << devU->errosString(err);

        err = devU->setChanelForSignal(minusBoard, minusChanelOnBoard, "M");
        qDebug() << "Status: " << devU->errosString(err);

        /* setup tester*/
        bool success = devTester->setFunction(model->index(i, TableViewConfigModel::COLUMN_FUNC).data().toString());
        /* if ACW -> set frequency*/
        if(devTester->getSignalFunction() == DeviceTester::ACW)
            err = devTester->setFrequencyForACW(model->index(i, TableViewConfigModel::COLUMN_FREQ).data().toString());
        /* ramp time*/
        success = devTester->setRampTime(model->index(i, TableViewConfigModel::COLUMN_RAMP).data().toString());
        /*set timer*/
        success = devTester->setTimer(model->index(i, TableViewConfigModel::COLUMN_TIME).data().toString(),
                                      model->index(i, TableViewConfigModel::COLUMN_FUNC).data().toString());
        /* set low current*/
        err = devTester->setLowCurrent(model->index(i, TableViewConfigModel::COLUMN_LCUR).data().toString(),
                                       model->index(i, TableViewConfigModel::COLUMN_FUNC).data().toString());
        /* set hi current*/
        err = devTester->setHiCurrent(model->index(i, TableViewConfigModel::COLUMN_HCUR).data().toString(),
                                       model->index(i, TableViewConfigModel::COLUMN_FUNC).data().toString());

        /* set voltage */
        err = devTester->setVoltage(model->index(i, TableViewConfigModel::COLUMN_VOLT).data().toString(),
                                       model->index(i, TableViewConfigModel::COLUMN_FUNC).data().toString());

        /* start teasting*/

        /* start timer read status  and block while testing*/
        timerReadInfo->start(500);
        timerCheck->start(3000);
        /* block while testing*/
        loop->exec();
        /* End main thread*/
        this->msleep(2000);
        err = devU->clearOutput(plusBoard);   // ToDo add if have not answer
        qDebug() << "Status: " << devU->errosString(err);
        err = devU->clearOutput(minusBoard);   // ToDo add if have not answer
        qDebug() << "Status: " << devU->errosString(err);
        emit statusProgress(progress*(i + 1));
    }

    emit statusProgress(100);
    emit finished();
}

void TestThread::slotTimerReadStatus()
{
    // read status tester and emit signal
    qDebug() << "Read status from Tester";

}

void TestThread::slotCheckEnd()
{
    // after time = rampTime + holdTime
    // stop timerReadStatus
    timerReadInfo->stop();
    qDebug() << "Stop read timer";
    loop->quit();
}
