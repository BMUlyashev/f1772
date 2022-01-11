#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new TableViewConfigModel();
    safeTester = new QList<SafeTester>();

    safeTester->append(SafeTester());
    safeTester->append(SafeTester());
    safeTester->append(SafeTester());
    model->populate(safeTester);
    ui->tableViewConfig->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->tableViewConfig->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewConfig->verticalHeader()->setDefaultSectionSize(15);
    ui->tableViewConfig->setModel(model);
    ui->tableViewConfig->setItemDelegateForColumn(TableViewConfigModel::COLUMN_STEP, new ReadOnlyDelegate(this));
    ui->tableViewConfig->setItemDelegateForColumn(TableViewConfigModel::COLUMN_PLUS, new ChanelDelegate(this));
    ui->tableViewConfig->setItemDelegateForColumn(TableViewConfigModel::COLUMN_MINUS, new ChanelDelegate(this));
    ui->tableViewConfig->setItemDelegateForColumn(TableViewConfigModel::COLUMN_FUNC, new FunctionDelegate(this));
    ui->tableViewConfig->setItemDelegateForColumn(TableViewConfigModel::COLUMN_FREQ, new FrequencyDelegate(this));
    ui->tableViewConfig->setItemDelegateForColumn(TableViewConfigModel::COLUMN_VOLT, new ValueDelegate(this));
    ui->tableViewConfig->setItemDelegateForColumn(TableViewConfigModel::COLUMN_RAMP, new ValueDelegate(this));
    ui->tableViewConfig->setItemDelegateForColumn(TableViewConfigModel::COLUMN_HCUR, new ValueDelegate(this));
    ui->tableViewConfig->setItemDelegateForColumn(TableViewConfigModel::COLUMN_LCUR, new ValueDelegate(this));
    ui->tableViewConfig->setItemDelegateForColumn(TableViewConfigModel::COLUMN_TIME, new ValueDelegate(this));
    ui->pBtnOpenConfigSteps->setEnabled(false);
    ui->pBtnSaveConfigSteps->setEnabled(false);
    ui->pBtnAddConfigSteps->setEnabled(false);
    ui->tableViewConfig->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pBtnEditConfigSteps_clicked()
{
    // Надо передать в модель, что разрешено редактирование

        ui->pBtnEditConfigSteps->setEnabled(false);
        ui->pBtnSaveConfigSteps->setEnabled(true);
        ui->pBtnAddConfigSteps->setEnabled(true);
        ui->pBtnOpenConfigSteps->setEnabled(false);
        ui->tableViewConfig->setEditTriggers(QAbstractItemView::AllEditTriggers);
        ui->pBtnNewConfigSteps->setText("Подтвердить");
        ui->pBtnLoadConfigToTest->setEnabled(false);
}


void MainWindow::on_pBtnNewConfigSteps_clicked()
{

    if (ui->pBtnNewConfigSteps->text() == "Новый")
    {
        // Стираем все данные и добавляем одну строчку
        model->clear();
        model->append(SafeTester());

    } else if (ui->pBtnNewConfigSteps->text() == "Подтвердить")
    {
        // Просто выходим и активируем кнопки
        ui->pBtnEditConfigSteps->setEnabled(true);
        ui->pBtnSaveConfigSteps->setEnabled(false);
        ui->pBtnAddConfigSteps->setEnabled(false);
        ui->pBtnOpenConfigSteps->setEnabled(true);
        ui->tableViewConfig->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->pBtnNewConfigSteps->setText("Новый");
        ui->pBtnLoadConfigToTest->setEnabled(true);

    }
}


void MainWindow::on_pBtnAddConfigSteps_clicked()
{
    // открываем окно добавления новых точек
   AddStepWindow addStepWindow(this);
   addStepWindow.setModal(true);
//   addStepWindow.exec();
    if (addStepWindow.exec())
    {
        qDebug() << QString("Your add %1 step").arg(addStepWindow.getCountValue());
        for (int count = 0; count < addStepWindow.getCountValue(); ++count)
        {
            model->append(SafeTester());
        }
    }
}

