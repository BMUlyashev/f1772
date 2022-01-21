#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    lblStatus = new QLabel(this);
    lblStatusTester = new QLabel(this);
    lblStatusU2270 = new QLabel(this);
    barStatus = new QProgressBar(this);

    ui->statusbar->setSizeGripEnabled(false);
    ui->statusbar->addPermanentWidget(lblStatusTester);
    ui->statusbar->addPermanentWidget(lblStatusU2270);
    ui->statusbar->addWidget(lblStatus);
    ui->statusbar->addWidget(new QLabel(),3);

    ui->statusbar->addPermanentWidget(barStatus, 1);
    lblStatusTester->setText("Подключение GPT-79803");
    lblStatusU2270->setText("Подключение У2270");

    lblStatus->setText("Ожидание");

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
    ui->pBtnOpenConfigSteps->setEnabled(true);
    ui->pBtnSaveConfigSteps->setEnabled(false);
    ui->pBtnAddConfigSteps->setEnabled(false);
    ui->tableViewConfig->setEditTriggers(QAbstractItemView::NoEditTriggers);

    devTester = new DeviceTester();
    devU = new DeviceU2270();
    testParamLoad = false;
    ui->pBtnStart->setEnabled(false);
    ui->pBtnStop->setEnabled(false);
    ui->pBtnSaveResult->setEnabled(false);
    ui->pBtnClearResult->setEnabled(false);

    setupTestTable();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete devTester;
    delete devU;
}

void MainWindow::on_pBtnEditConfigSteps_clicked()
{
    // Надо передать в модель, что разрешено редактирование
        testParamLoad = false;
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

void MainWindow::saveModelData(QString fileName)
{
    // open the file for writing
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("configuration");   // Write the first element of his name

    qDebug() << model->rowCount();
    for (int count = 0; count < model->rowCount() ; ++count)
    {
//        qDebug() << model->index(count, 2).data().toString();
        xmlWriter.writeStartElement("step");
        xmlWriter.writeAttribute("n", model->index(count, TableViewConfigModel::COLUMN_STEP).data().toString());
        xmlWriter.writeAttribute("plus", model->index(count, TableViewConfigModel::COLUMN_PLUS).data().toString());
        xmlWriter.writeAttribute("minus", model->index(count, TableViewConfigModel::COLUMN_MINUS).data().toString());
        xmlWriter.writeAttribute("func", model->index(count, TableViewConfigModel::COLUMN_FUNC).data().toString());
        xmlWriter.writeAttribute("freq", model->index(count, TableViewConfigModel::COLUMN_FREQ).data().toString());
        xmlWriter.writeAttribute("volt", model->index(count, TableViewConfigModel::COLUMN_VOLT).data().toString());
        xmlWriter.writeAttribute("lcur", model->index(count, TableViewConfigModel::COLUMN_LCUR).data().toString());
        xmlWriter.writeAttribute("hcur", model->index(count, TableViewConfigModel::COLUMN_HCUR).data().toString());
        xmlWriter.writeAttribute("rampt", model->index(count, TableViewConfigModel::COLUMN_RAMP).data().toString());
        xmlWriter.writeAttribute("timet", model->index(count, TableViewConfigModel::COLUMN_TIME).data().toString());
        xmlWriter.writeEndElement();        // Закрываем тег
    }
    xmlWriter.writeEndElement();                    // End element configuration
    xmlWriter.writeEndDocument();
    file.close();
}

void MainWindow::on_pBtnSaveConfigSteps_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранение конфигурации", ".",
                                                    "Xml files (*.xml)");
    if(fileName != "")
    {
        // saving
        qDebug() << "Saving into " << fileName;
        saveModelData(fileName);
    }

}

void MainWindow::loadModelData(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Ошибка файла", "Не удалось открыть файл", QMessageBox::Ok);
    } else
    {
        // reading configuration
        model->clear(); // clear model
        QString step, plus, minus, func, freq, volt, lcur, hcur, rampt, timet;
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&file);
        xmlReader.readNext();
        while(!xmlReader.atEnd())
        {
            if (xmlReader.isStartElement())
            {
                if(xmlReader.name() == "step")
                {
                    // parsing data to model
                    foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    {
                        if(attr.name().toString() == "n"){step = attr.value().toString();}
                        if(attr.name().toString() == "plus"){plus = attr.value().toString();}
                        if(attr.name().toString() == "minus"){minus = attr.value().toString();}
                        if(attr.name().toString() == "func"){func = attr.value().toString();}
                        if(attr.name().toString() == "freq"){freq = attr.value().toString();}
                        if(attr.name().toString() == "volt"){volt = attr.value().toString();}
                        if(attr.name().toString() == "hcur"){hcur = attr.value().toString();}
                        if(attr.name().toString() == "lcur"){lcur = attr.value().toString();}
                        if(attr.name().toString() == "rampt"){rampt = attr.value().toString();}
                        if(attr.name().toString() == "timet"){timet = attr.value().toString();}
                    }
                    qDebug() << "Step =" << step << "PlusCh =" << plus << "Minus =" << minus <<
                                "Func =" << func << "Freq =" << freq << "Volt =" << volt <<
                                "LCur =" << lcur << "HCur =" << hcur <<
                                "Hold Time =" << timet << "Rise Time =" << rampt;
//                    model->append(SafeTester(plus, minus,))
                    SafeTester tmpTester;
                    tmpTester.setPlusChanel(plus.toInt());
                    tmpTester.setMinusChanel(minus.toInt());
                    tmpTester.setVoltFunction(func == "DCW" ? SafeTester::DCW : SafeTester::ACW);
                    tmpTester.setFrequencyACWVoltage(freq == "60" ? SafeTester::FREQ_60HZ : SafeTester::FREQ_50HZ);
                    tmpTester.setLowCurrentValue(lcur.toDouble());
                    tmpTester.setHiCurrentValue(hcur.toDouble());
                    tmpTester.setRampTime(rampt.toDouble());
                    tmpTester.setTimerTime(timet.toDouble());
                    model->append(tmpTester);
                }
            }
            xmlReader.readNext();
        }
        file.close();
    }


}
void MainWindow::on_pBtnOpenConfigSteps_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть конфигурацию", ".",
                                                    "Xml files (*.xml)");
    if (fileName != ""){loadModelData(fileName);}
}


void MainWindow::on_actionSerialPort_triggered()
{
    //SerialPortWindow serialPortWindow(this);
    SerialPortWindow serialPortWindow(this, devTester, devU);
    serialPortWindow.setModal(true);
    if(serialPortWindow.exec() == QDialog::Accepted)
    {
        // Занимаем порты и выводим информацию статуcбар
        if(devTester->isConnected())
            lblStatusTester->setText(QString("GPT-79803 (%1)").arg(devTester->getPortName()));
        if(devU->isConnected())
            lblStatusU2270->setText(QString("У2270 (%1)").arg(devU->getPortName()));
        if (devTester->isConnected() && devU->isConnected())
        {
            ui->pBtnStart->setEnabled(true);
            ui->pBtnSaveResult->setEnabled(true);
            ui->pBtnClearResult->setEnabled(true);
        }
    }


}


void MainWindow::on_pBtnLoadConfigToTest_clicked()
{
    // maybe check number of chanell were not equal
    for (int count = 0; count < model->rowCount() ; ++count)
    {
        if(model->index(count, TableViewConfigModel::COLUMN_PLUS).data().toString()  ==
           model->index(count, TableViewConfigModel::COLUMN_MINUS).data().toString() )
        {
            QMessageBox::critical(
                        this,
                        "Ошибка",
                        QString("Одинаковые номера каналов:\nШаг № %1").arg(model->index(count, TableViewConfigModel::COLUMN_STEP).data().toString()));
        }  else
        {
            // unblock buttons on tester screen
            testParamLoad = true;
            ui->tabWidget->setCurrentIndex(0);
        }
    }
}


void MainWindow::on_pBtnStart_clicked()
{
    if(testParamLoad)
    {
        // start thread
    } else
    {
        QMessageBox::warning(this, "Внимание", "Параметры тестирования не загружены\nв память!");
    }
}

void MainWindow::setupTestTable()
{

    ui->tableWidget->verticalHeader()->setDefaultSectionSize(15);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->setColumnWidth(0,15);
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << trUtf8("Шаг") << trUtf8("Сигнал") <<
                                               "Частота, Гц" << "Тестовое\nнапряжение,кВ" << "Ток мин.,мА" <<
                                               "Ток макс.,мА" << "Время\nнарастания,с" << "Время\nудержания,с" <<
                                               "Результат" << "Статус");
    //ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableWidget->
}
