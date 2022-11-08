#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "safetester.h"
#include "tableviewconfigmodel.h"
#include "channelnamemodel.h"

#include "addstepwindow.h"

#include "serialportwindow.h"

#include "devicetester.h"
#include "deviceu2270.h"

#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QMessageBox>
#include <QProgressBar>
#include <QLabel>
#include <QCheckBox>
//#include "testthread.h"
#include <QThread>

#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Save model data in xml file
    void saveModelData(QString fileName);
    void loadModelData(QString fileName);

private:
    void setupTestTable();
private slots:

    void on_pBtnEditConfigSteps_clicked();
    void on_pBtnNewConfigSteps_clicked();
    void on_pBtnAddConfigSteps_clicked();
    void on_pBtnSaveConfigSteps_clicked();
    void on_pBtnOpenConfigSteps_clicked();
    void on_actionSerialPort_triggered();
    void on_pBtnLoadConfigToTest_clicked();
    void on_pBtnStart_clicked();
    void threadFinished();
    void statusStepPreparation(int); /*Вывод информаци в mainwondow о точках (номер шага)*/
    void statusProgress(int, int);       /* for progress bar from thread*/
    void statusMeasure(DeviceTester::Measure);
    void on_pBtnStop_clicked();

    void on_pBtnClearResult_clicked();
    void changeSetupModel();

    void on_pBtnSaveResult_clicked();

private:
    Ui::MainWindow *ui;

    QLabel *lblStatusTester;
    QLabel *lblStatusU2270;
    QLabel *lblStatus;
    QProgressBar *barStatus;
//    SafeTester *tester; // Для отладки
    QList<SafeTester> *safeTester;
    TableViewConfigModel *model;
    ChannelNameModel *modelChanelName;
//    CommunicationDevice *tester;
    DeviceTester *devTester;
    DeviceU2270 *devU;
    bool testParamLoad;
    Worker *m_worker;
    QThread *m_thread;
    int currentRow;
    QProgressBar *bar;
    QCheckBox *checkContinue;

};
#endif // MAINWINDOW_H
