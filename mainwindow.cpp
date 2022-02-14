#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QHeaderView>
#include <QPair>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //lblStatus = new QLabel(this);
    lblStatusTester = new QLabel(this);
    lblStatusU2270 = new QLabel(this);
    barStatus = new QProgressBar(this);
    barStatus->setTextVisible(false);
    checkContinue = new QCheckBox(this);
    checkContinue->setText("Остановить в случае пробоя изоляции");
    checkContinue->setChecked(true);
    ui->statusbar->addWidget(checkContinue);

    ui->statusbar->setSizeGripEnabled(false);
    ui->statusbar->addPermanentWidget(lblStatusTester);
    ui->statusbar->addPermanentWidget(lblStatusU2270);
    //ui->statusbar->addWidget(lblStatus);
    ui->statusbar->addWidget(new QLabel(),3);


    ui->statusbar->addPermanentWidget(barStatus, 1);
    lblStatusTester->setText("Подключение GPT-79803");
    lblStatusU2270->setText("Подключение У2270");

//    lblStatus->setText("Ожидание");
    modelChanelName = new ChannelNameModel();
    model = new TableViewConfigModel();
    safeTester = new QList<SafeTester>();

    /* test append modelChanelName*/
//    modelChanelName->append(4, "ADC#0");
//    modelChanelName->append(1, "ADC#0");
//    modelChanelName->append(4, "ADC#0");
//    modelChanelName->append(2, "ADC#0");

    connect(model, SIGNAL(modelChanged()), this, SLOT(changeSetupModel()));
    ui->tableChannelName->setItemDelegateForColumn(0, new ReadOnlyDelegate);
    ui->tableChannelName->setItemDelegateForColumn(1, new NameDelegate(modelChanelName->getStringNames()));
    //qsort(modelChanelName->data())
    ui->tableChannelName->setModel(modelChanelName);
    ui->tableChannelName->verticalHeader()->setDefaultSectionSize(15);
    ui->tableChannelName->horizontalHeader()->setStretchLastSection(true);

    ui->tableChannelName->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->tableChannelName->setColumnWidth(0,40);
    /**/
    safeTester->append(SafeTester());
//    safeTester->append(SafeTester());
//    safeTester->append(SafeTester());
    model->populate(safeTester);
    changeSetupModel();
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
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    qRegisterMetaType<DeviceTester::Measure>("DeviceTester::Measure");

    m_worker = new Worker(0, devTester, devU, model);
    m_thread = new QThread;
    m_worker->moveToThread(m_thread);
    connect(m_worker, SIGNAL(statusPreparation(int)), this, SLOT(statusStepPreparation(int)));
    connect(m_worker, SIGNAL(finished()), this, SLOT(threadFinished()));
    connect(m_thread, SIGNAL(started()), m_worker, SLOT(run()));
    //connect(m_worker, SIGNAL(finished()), m_thread, SLOT(quit()));
    //connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));

    connect(m_worker, SIGNAL(measure(DeviceTester::Measure)), this, SLOT(statusMeasure(DeviceTester::Measure)));
    ui->tableWidget->setItemDelegateForColumn(11, new ProgressBarDelegate);


}

MainWindow::~MainWindow()
{
    //m_workerThread->quit();
    //m_workerThread->wait();
    delete ui;
    delete devTester;
    delete devU;
    delete m_worker;

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
        //  TODO: проверить на совпадение каналов.
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
    xmlWriter.writeStartElement("steps");   // Write the first element of his name
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
    xmlWriter.writeEndElement();
    // TODO сохранить наименования каналов
    /* save names channels*/
    xmlWriter.writeStartElement("names");
        for (int i = 0; i < modelChanelName->rowCount();++i)
        {
            xmlWriter.writeStartElement("name");
            xmlWriter.writeAttribute("channel", modelChanelName->index(i,0).
                                     data().toString());
            xmlWriter.writeAttribute("name", modelChanelName->index(i,1).
                                     data().toString());
            xmlWriter.writeEndElement();
        }
    xmlWriter.writeEndElement();
    /**/
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
//        modelChanelName->clear();
        QString step, plus, minus, func, freq, volt, lcur, hcur, rampt, timet;
        QString channel, name;
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
                        if(attr.name().toString() == "n"){step = attr.value().toString();continue;}
                        if(attr.name().toString() == "plus"){plus = attr.value().toString();continue;}
                        if(attr.name().toString() == "minus"){minus = attr.value().toString();continue;}
                        if(attr.name().toString() == "func"){func = attr.value().toString();continue;}
                        if(attr.name().toString() == "freq"){freq = attr.value().toString();continue;}
                        if(attr.name().toString() == "volt"){volt = attr.value().toString();continue;}
                        if(attr.name().toString() == "hcur"){hcur = attr.value().toString();continue;}
                        if(attr.name().toString() == "lcur"){lcur = attr.value().toString();continue;}
                        if(attr.name().toString() == "rampt"){rampt = attr.value().toString();continue;}
                        if(attr.name().toString() == "timet"){timet = attr.value().toString();continue;}
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
                if(xmlReader.name() == "name")
                {
                    qDebug() << "Read names";
                    foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                    {
                        if(attr.name().toString() == "channel"){channel = attr.value().toString();continue;}
                        if(attr.name().toString() == "name"){name = attr.value().toString();continue;}
                    }
                    qDebug() << QString("Channel %1 has name %2").arg(channel).arg(name);
//                    modelChanelName->append(channel.toInt(), name);
                    /* check the name in model*/
                    for (int i=0; i < modelChanelName->rowCount();++i)
                    {
                        if(modelChanelName->index(i,0).data().toInt() == channel.toInt())
                        {
                            modelChanelName->setChannelName(i, name);
                            break;
                        }
                    }
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
            break;
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
        m_worker->m_stop = false;
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << trUtf8("Шаг") << trUtf8("Канал(+)") << trUtf8("Канал(-)")
                                                   << trUtf8("Сигнал") << "Частота, Гц" << "Тестовое\nнапряжение,кВ"
                                                   << "Ток мин.,мА" << "Ток макс.,мА" << "Время\nнарастания,с"
                                                   << "Время\nудержания,с" << "Результат" << "Статус");
//        ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        ui->pBtnStart->setEnabled(false);
        ui->pBtnSaveResult->setEnabled(false);
        ui->pBtnClearResult->setEnabled(false);
        barStatus->setValue(0);
//        mThread->start();
        m_thread->start();
        ui->pBtnStop->setEnabled(true);
    } else
    {
        QMessageBox::warning(this, "Внимание", "Параметры тестирования не загружены\nв память!");
    }
}

void MainWindow::setupTestTable()
{

    ui->tableWidget->verticalHeader()->setDefaultSectionSize(15);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(false);
    ui->tableWidget->setColumnCount(12);
    ui->tableWidget->setColumnWidth(0,10);
    ui->tableWidget->setColumnWidth(1, 60);
    ui->tableWidget->setColumnWidth(2, 60);
    ui->tableWidget->setColumnWidth(3, 60);
    ui->tableWidget->setColumnWidth(4, 70);
    ui->tableWidget->setColumnWidth(5, 90);
    ui->tableWidget->setColumnWidth(6, 80);
    ui->tableWidget->setColumnWidth(7, 80);
    ui->tableWidget->setColumnWidth(8, 80);
    ui->tableWidget->setColumnWidth(9, 80);
    ui->tableWidget->setColumnWidth(10, 140);

    ui->tableWidget->setColumnWidth(11, 120);
//    ui->tableWidget->setColumnWidth(1,25);
//    ui->tableWidget->setColumnWidth(2,25);
    ui->tableWidget->horizontalHeader()->setVisible(true);
    //ui->tableWidget->setRowCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << trUtf8("Шаг") << trUtf8("Канал(+)") << trUtf8("Канал(-)")
                                               << trUtf8("Сигнал") << "Частота, Гц" << "Тестовое\nнапряжение,кВ"
                                               << "Ток мин.\nмА" << "Ток макс.\nмА" << "Время\nнарастания,с"
                                               << "Время\nудержания,с" << "Измерения" << "Статус");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(9,QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(11,QHeaderView::Fixed);
    //ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableWidget->
}

void MainWindow::threadFinished()
{
    ui->pBtnStart->setEnabled(true);
    ui->pBtnStop->setEnabled(false);
    ui->pBtnClearResult->setEnabled(true);
    ui->pBtnSaveResult->setEnabled(true);
    m_thread->terminate();
}

void MainWindow::statusStepPreparation(int a)
{
    ui->tableWidget->insertRow(a);
    currentRow = a;
    ui->tableWidget->setItem(a, TableViewConfigModel::COLUMN_STEP,
                             new QTableWidgetItem(model->index(a, TableViewConfigModel::COLUMN_STEP).data().toString()));
    ui->tableWidget->setItem(a, TableViewConfigModel::COLUMN_PLUS,
                             new QTableWidgetItem(model->index(a, TableViewConfigModel::COLUMN_PLUS).data().toString()));
    ui->tableWidget->setItem(a, TableViewConfigModel::COLUMN_MINUS,
                             new QTableWidgetItem(model->index(a, TableViewConfigModel::COLUMN_MINUS).data().toString()));
    ui->tableWidget->setItem(a, TableViewConfigModel::COLUMN_FUNC,
                             new QTableWidgetItem(model->index(a, TableViewConfigModel::COLUMN_FUNC).data().toString()));
    ui->tableWidget->setItem(a, TableViewConfigModel::COLUMN_FREQ,
                             new QTableWidgetItem(model->index(a, TableViewConfigModel::COLUMN_FREQ).data().toString()));
    ui->tableWidget->setItem(a, TableViewConfigModel::COLUMN_VOLT,
                             new QTableWidgetItem(model->index(a, TableViewConfigModel::COLUMN_VOLT).data().toString()));
    ui->tableWidget->setItem(a, TableViewConfigModel::COLUMN_LCUR,
                             new QTableWidgetItem(model->index(a, TableViewConfigModel::COLUMN_LCUR).data().toString()));
    ui->tableWidget->setItem(a, TableViewConfigModel::COLUMN_HCUR,
                             new QTableWidgetItem(model->index(a, TableViewConfigModel::COLUMN_HCUR).data().toString()));
    ui->tableWidget->setItem(a, TableViewConfigModel::COLUMN_RAMP,
                             new QTableWidgetItem(model->index(a, TableViewConfigModel::COLUMN_RAMP).data().toString()));
    ui->tableWidget->setItem(a, TableViewConfigModel::COLUMN_TIME,
                             new QTableWidgetItem(model->index(a, TableViewConfigModel::COLUMN_TIME).data().toString()));

    ui->tableWidget->item(a,TableViewConfigModel::COLUMN_STEP)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(a,TableViewConfigModel::COLUMN_PLUS)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(a,TableViewConfigModel::COLUMN_MINUS)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(a,TableViewConfigModel::COLUMN_FUNC)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(a,TableViewConfigModel::COLUMN_FREQ)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(a,TableViewConfigModel::COLUMN_VOLT)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(a,TableViewConfigModel::COLUMN_LCUR)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(a,TableViewConfigModel::COLUMN_HCUR)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(a,TableViewConfigModel::COLUMN_RAMP)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(a,TableViewConfigModel::COLUMN_TIME)->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(currentRow,10,
                             new QTableWidgetItem(QString("0.000kV     0.000 mA")));
    ui->tableWidget->item(currentRow, 10)->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem *item = new QTableWidgetItem("Настройка...");
    ui->tableWidget->setItem(currentRow, 11, item);
    ui->tableWidget->item(currentRow, 11)->setTextAlignment(Qt::AlignCenter);



}

void MainWindow::statusProgress(int value)
{
    barStatus->setValue(value);
}
void MainWindow::statusMeasure(DeviceTester::Measure data)
{
    QString statusTester = "Ожидание";

    if (data.status == "TEST ")
        statusTester = "Тестирование";
    if (data.status == "PASS ")
    {
        statusTester = "Успешно";
        data.progress = 100;
    }
    if (data.status == "FAIL "){
        statusTester = "Облом";
        data.progress = 100;
        if (checkContinue->isChecked())
        {
            m_worker->m_stop = true;
        }
    }
//    QProgressBar stepProgress;
    ui->tableWidget->setItem(currentRow,10,
                             new QTableWidgetItem(data.voltageValue + "     " + data.currentValue));
    ui->tableWidget->item(currentRow, 10)->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem *item = ui->tableWidget->item(currentRow,11);
    item->setData(Qt::DisplayRole, QString(statusTester));
    item->setData(Qt::UserRole, data.progress);
}

void MainWindow::on_pBtnStop_clicked()
{
    m_worker->m_stop = true;
}


void MainWindow::on_pBtnClearResult_clicked()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    setupTestTable();
}

void MainWindow::changeSetupModel()
{
    qDebug() << "Setup Model changes";
    /* сохранить старые наименования для каналов*/
    QVector<QPair<int, QString>> tmpPair;
    QVector<QString> nameChannels;
    for(int i = 0; i < modelChanelName->rowCount(); ++i)
    {
        tmpPair.append(QPair<int,QString>(modelChanelName->index(i,0).data().toInt(),
                                          modelChanelName->index(i,1).data().toString()));
        if (nameChannels.size() <= modelChanelName->index(i,0).data().toInt())
        {
            nameChannels.resize(modelChanelName->index(i,0).data().toInt()+1);
        }
        nameChannels.replace(modelChanelName->index(i,0).data().toInt(),
                            modelChanelName->index(i,1).data().toString());

    }
    modelChanelName->clear();
    for (int j = 1; j<= 100; ++j)
    {
        for (int i = 1; i <= model->rowCount(); ++i)
        {
            if((model->index(i-1,TableViewConfigModel::COLUMN_PLUS).data().toInt() == j) ||
               (model->index(i-1,TableViewConfigModel::COLUMN_MINUS).data().toInt() == j))
            {
                qDebug() << QString("Channel %1 is present.").arg(j);
                if(j < nameChannels.size())
                {
                    if(nameChannels.at(j) !="")
                        modelChanelName->append(j,nameChannels.at(j));
                    else
                        modelChanelName->append(j, "Имя канала");
                } else
                    modelChanelName->append(j, "Имя канала");
                break;
            }
        }

    }   // test new
}
