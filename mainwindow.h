#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "safetester.h"
#include "tableviewconfigmodel.h"
#include "addstepwindow.h"

#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QMessageBox>

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

private:
    Ui::MainWindow *ui;
//    SafeTester *tester; // Для отладки
    QList<SafeTester> *safeTester;
    TableViewConfigModel *model;


};
#endif // MAINWINDOW_H
