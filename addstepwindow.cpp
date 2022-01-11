#include "addstepwindow.h"
#include "ui_addstepwindow.h"
#include <QRegExpValidator>

AddStepWindow::AddStepWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStepWindow)
{
    ui->setupUi(this);
//    QRegExp regExp("[0-9]{0,100}");
    ui->lineEdit->setValidator(new QIntValidator(0,100, this));
}

AddStepWindow::~AddStepWindow()
{
    delete ui;
}

void AddStepWindow::setCountValue(int value)
{
    countValue = value;
}

int AddStepWindow::getCountValue()
{
    return countValue;
}

void AddStepWindow::on_buttonBox_accepted()
{
    setCountValue(ui->lineEdit->text().toInt());
}

