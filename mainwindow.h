#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "safetester.h"
#include "tableviewconfigmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pBtnEditConfigSteps_clicked();

    void on_pBtnNewConfigSteps_clicked();

private:
    Ui::MainWindow *ui;
//    SafeTester *tester; // Для отладки
    QList<SafeTester> *safeTester;
    TableViewConfigModel *model;

};
#endif // MAINWINDOW_H
