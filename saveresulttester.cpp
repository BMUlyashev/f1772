#include "saveresulttester.h"
#include "ui_saveresulttester.h"

SaveResultTester::SaveResultTester(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveResultTester)
{
    ui->setupUi(this);
}

SaveResultTester::~SaveResultTester()
{
    delete ui;
}
