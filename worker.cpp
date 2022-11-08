#include "worker.h"
#include <QDebug>
#include <QThread>
#include <QEventLoop>
#include <QElapsedTimer>


Worker::Worker(QObject *parent) : QObject(parent)
{


}

Worker::Worker(QObject *parent, DeviceTester *devTester, DeviceU2270 *devU, TableViewConfigModel *model)
            : QObject(parent)
{
    this->devTester = devTester;
    this->model = model;
    this->devU = devU;
    m_stop = false;

    connect(this, SIGNAL(closeSerial()), devTester, SLOT(closeSerial()));
    connect(this, SIGNAL(openSerial()), devTester, SLOT(openSerial()));

}
void Worker::slotTimerReadStatus()
{
    // read status tester and emit signal
    qDebug() << "Read status from Tester";
    DeviceTester::Measure data;
//    QElapsedTimer tim;
//    tim.start();
    int status = devTester->readMeasure(&data);
    /* value for progress */
    static double r_time;
    double t_time = 0.0;
    QStringList tmpTime = data.time.split('=');
    if (tmpTime.length() == 2)
    {
        QStringList m_timeValue = tmpTime.at(1).split('S');
        if(tmpTime.at(0) == "R")
        {
            r_time = m_timeValue.at(0).toDouble();
        }
        if(tmpTime.at(0) == "T")
        {
            t_time = m_timeValue.at(0).toDouble();
        }
    }
    qDebug() << QString("Time: R=%1 S T=%2 S ").arg(r_time).arg(t_time);
    data.progress = 100 * ((r_time + t_time)/m_totalTime);
    emit measure(data);
    leftTime -= 500/1000.0;
    qDebug() << "Left time = " << leftTime;
    emit statusStepProgress(totalTime, leftTime);
    if(data.status == "FAIL ")
    {
        // stop main timer
        qDebug() << "Testing FAIL!!!";
        emit statusFail();

    }

}

void Worker::slotTestingFail()
{

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
    totalTime = 0;
    for (int i = 0; i < model->rowCount(); i++) {
        int a = model->index(i, TableViewConfigModel::COLUMN_RAMP).data().toDouble() ;
        int b = model->index(i, TableViewConfigModel::COLUMN_TIME).data().toDouble();
        int prepairTime = 3;
        totalTime += a + b + prepairTime;
    }
    leftTime = totalTime;
    emit(statusStepProgress(totalTime, leftTime));
    int progress = (100 / model->rowCount());
    int plusBoard, minusBoard, plusChanel, minusChanel, plusChanelOnBoard, minusChanelOnBoard;
    int err;
    int countAttemts = 1;
    QStringList boards; // stores num of COM boards
    for (int i = 1; i <=5; ++i)
    {
        err = devU->getTypeBoard(i);
        if (err == DeviceU2270::OK) {
            boards.append(QString::number(i));
            qDebug() << QString("Board %1 is OK").arg(i);
        }
        else {
            qDebug() << QString("Error reading type U2270 boards: %1").arg(i);
        }
    }
    /* read number of boards/ Check 5 boards*/
    /* clear all boards*/
    QElapsedTimer timeOperation;
    timeOperation.start();
    for (int i = 0; i < boards.length(); ++i) {
        qDebug() << QString("Clear board %1. Attemp # %2").arg(boards.at(i)).arg(countAttemts);
        err = devU->clearOutput(boards.at(i).toInt());   // ToDo add if have not answer
            // check for answer errors
        qDebug() << "Status: " << devU->errosString(err);
    }
    leftTime -= timeOperation.elapsed()/1000.0;
    emit(statusStepProgress(totalTime, leftTime));

    for (int i = 0; i < model->rowCount(); ++i){
        if (m_stop) break;

        timeOperation.restart();
//        emit(statusStepProgress(totalTime, ));
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

        QElapsedTimer prTime;
        prTime.start();
        plusChanel = model->index(i, TableViewConfigModel::COLUMN_PLUS).data().toInt();
        plusBoard = (plusChanel - 1)/10 + 1;
        plusChanelOnBoard = !(plusChanel % 10) ? 10 : (plusChanel % 10);
        minusChanel = model->index(i, TableViewConfigModel::COLUMN_MINUS).data().toInt();
        minusBoard = (minusChanel - 1)/10 + 1;
        minusChanelOnBoard = !(minusChanel % 10) ? 10 : (minusChanel % 10);
        m_totalTime = model->index(i, TableViewConfigModel::COLUMN_RAMP).data().toDouble()
                + model->index(i, TableViewConfigModel::COLUMN_TIME).data().toDouble();

        leftTime -= timeOperation.elapsed()/1000.0;
        emit(statusStepProgress(totalTime, leftTime));
        timeOperation.restart();

        qDebug() << QString("Clear (plus) board %1. Attemp # %2").arg(plusBoard).arg(countAttemts);
        err = devU->clearOutput(plusBoard);   // ToDo add if have not answer

        leftTime -= timeOperation.elapsed()/1000.0;
        emit(statusStepProgress(totalTime, leftTime));
        timeOperation.restart();

            // check for answer errors
        qDebug() << "Status: " << devU->errosString(err);

        qDebug() << QString("Clear (minus) board %1. Attemp # %2").arg(plusBoard).arg(countAttemts);
        err = devU->clearOutput(minusBoard);   // ToDo add if have not answer
            // check for answer errors
        qDebug() << "Status: " << devU->errosString(err);

        leftTime -= timeOperation.elapsed()/1000.0;
        emit(statusStepProgress(totalTime, leftTime));
        timeOperation.restart();

        /* set U2270 for sihnals*/
        err = devU->setChanelForSignal(plusBoard, plusChanelOnBoard, "P");

        leftTime -= timeOperation.elapsed()/1000.0;
        emit(statusStepProgress(totalTime, leftTime));
        timeOperation.restart();

        qDebug() << "Status: " << devU->errosString(err);

        err = devU->setChanelForSignal(minusBoard, minusChanelOnBoard, "M");
        qDebug() << "Status: " << devU->errosString(err);

        leftTime -= timeOperation.elapsed()/1000.0;
        emit(statusStepProgress(totalTime, leftTime));
        timeOperation.restart();

        /* setup tester*/
        bool success = devTester->setFunction(model->index(i, TableViewConfigModel::COLUMN_FUNC).data().toString());
        leftTime -= timeOperation.elapsed()/1000.0;
        emit(statusStepProgress(totalTime, leftTime));
        timeOperation.restart();
        /* if ACW -> set frequency*/
        if(devTester->getSignalFunction() == DeviceTester::ACW)
            err = devTester->setFrequencyForACW(model->index(i, TableViewConfigModel::COLUMN_FREQ).data().toString());
        /* ramp time*/
        leftTime -= timeOperation.elapsed()/1000.0;
        emit(statusStepProgress(totalTime, leftTime));
        timeOperation.restart();

        success = devTester->setRampTime(model->index(i, TableViewConfigModel::COLUMN_RAMP).data().toString());
        leftTime -= timeOperation.elapsed()/1000.0;
        emit(statusStepProgress(totalTime, leftTime));
        timeOperation.restart();

        /*set timer*/
        success = devTester->setTimer(model->index(i, TableViewConfigModel::COLUMN_TIME).data().toString(),
                                      model->index(i, TableViewConfigModel::COLUMN_FUNC).data().toString());
        leftTime -= timeOperation.elapsed()/1000.0;
        emit(statusStepProgress(totalTime, leftTime));
        timeOperation.restart();

        /* set low current*/
        err = devTester->setLowCurrent(model->index(i, TableViewConfigModel::COLUMN_LCUR).data().toString(),
                                       model->index(i, TableViewConfigModel::COLUMN_FUNC).data().toString());
        leftTime -= timeOperation.elapsed()/1000.0;
        emit(statusStepProgress(totalTime, leftTime));
        timeOperation.restart();

        /* set hi current*/
        err = devTester->setHiCurrent(model->index(i, TableViewConfigModel::COLUMN_HCUR).data().toString(),
                                       model->index(i, TableViewConfigModel::COLUMN_FUNC).data().toString());
        leftTime -= timeOperation.elapsed()/1000.0;
        emit(statusStepProgress(totalTime, leftTime));
        timeOperation.restart();

        /* set voltage */
        err = devTester->setVoltage(model->index(i, TableViewConfigModel::COLUMN_VOLT).data().toString(),
                                       model->index(i, TableViewConfigModel::COLUMN_FUNC).data().toString());
        leftTime -= timeOperation.elapsed()/1000.0;
        emit(statusStepProgress(totalTime, leftTime));
        timeOperation.restart();

        /* Start testing */
        err = devTester->startTesting(true);

        leftTime -= timeOperation.elapsed()/1000.0;
        emit(statusStepProgress(totalTime, leftTime));
//        timeOperation.restart();

        qDebug() << "Time for setup " << prTime.elapsed() << " milliseconds";
        /* wait until is test*/
        QTimer timerStatus;
        connect(&timerStatus, SIGNAL(timeout()), this, SLOT(slotTimerReadStatus()));
        QTimer timerWait;
        QEventLoop loop;
        connect(&timerWait, SIGNAL(timeout()), &loop, SLOT(quit()));
        connect(&timerWait, SIGNAL(timeout()), this, SLOT(slotCheckEnd()));
        connect(&timerWait, SIGNAL(timeout()), &timerStatus, SLOT(stop()));

        connect(this, SIGNAL(statusFail()), &timerStatus, SLOT(stop()));
        connect(this, SIGNAL(statusFail()), &timerWait, SLOT(stop()));
        connect(this, SIGNAL(statusFail()), &loop, SLOT(quit()));
        int waitTime = model->index(i, TableViewConfigModel::COLUMN_TIME).data().toDouble() + model->index(i, TableViewConfigModel::COLUMN_RAMP).data().toDouble();
        timerWait.start(waitTime*1000 + 500); // waitTime in seconds, but argument should be in milliseconds
        timerStatus.start(500);
        loop.exec();

        slotTimerReadStatus();  // контрольное считывание о статусе проверки
        /* set output channel to NULL ??????*/
        /* set testing to false ??????*/
        qDebug() << "Left time after test = " << leftTime;
        qDebug() << QString("Clear (plus) board %1. Attemp # %2").arg(plusBoard).arg(countAttemts);
        err = devU->clearOutput(plusBoard);   // ToDo add if have not answer
            // check for answer errors
        qDebug() << "Status: " << devU->errosString(err);

        qDebug() << QString("Clear (minus) board %1. Attemp # %2").arg(plusBoard).arg(countAttemts);
        err = devU->clearOutput(minusBoard);   // ToDo add if have not answer
            // check for answer errors
        qDebug() << "Status: " << devU->errosString(err);

    }
    leftTime = 0.0;
    emit(statusStepProgress(totalTime, leftTime));
    err = devTester->startTesting(false);
    /* clear all boards */
    for (int i = 0; i < boards.length(); ++i) {
        qDebug() << QString("Clear board %1. Attemp # %2").arg(boards.at(i)).arg(countAttemts);
        err = devU->clearOutput(boards.at(i).toInt());   // ToDo add if have not answer
            // check for answer errors
        qDebug() << "Status: " << devU->errosString(err);
    }
     emit finished();
    /* отключаем тестирование и выключаем все каналы*/

}
