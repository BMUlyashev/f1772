#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tester = new SafeTester(SafeTester::ACW, SafeTester::FREQ_50HZ, "0.100");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    qDebug() << "tester:-> " << tester->getVoltFunctionName() << " "
             << tester->getFrequencyACWVoltage() << " "
             << tester->getVoltageValue();
}


void MainWindow::on_pushButton_2_clicked()
{
    tester->setFrequencyACWVoltage(SafeTester::FREQ_60HZ);
    tester->setVoltFunction(SafeTester::DCW);
    tester->setVoltageValue(0.500);
}

