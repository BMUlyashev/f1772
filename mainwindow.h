#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "safetester.h"
#include "tableviewconfigmodel.h"
#include "addstepwindow.h"

#include "serialportwindow.h"

#include "devicetester.h"

#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QMessageBox>
#include <QProgressBar>
#include <QLabel>


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
private slots:

    void on_pBtnEditConfigSteps_clicked();

    void on_pBtnNewConfigSteps_clicked();

    void on_pBtnAddConfigSteps_clicked();

    void on_pBtnSaveConfigSteps_clicked();

    void on_pBtnOpenConfigSteps_clicked();

    void on_actionSerialPort_triggered();

private:
    Ui::MainWindow *ui;

    QLabel *lblStatusTester;
    QLabel *lblStatusU2270;
    QLabel *lblStatus;
    QProgressBar *barStatus;
//    SafeTester *tester; // Для отладки
    QList<SafeTester> *safeTester;
    TableViewConfigModel *model;
//    CommunicationDevice *tester;

};
#endif // MAINWINDOW_H
