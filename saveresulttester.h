#ifndef SAVERESULTTESTER_H
#define SAVERESULTTESTER_H

#include <QDialog>

namespace Ui {
class SaveResultTester;
}

class SaveResultTester : public QDialog
{
    Q_OBJECT

public:
    explicit SaveResultTester(QWidget *parent = nullptr);
    ~SaveResultTester();

private:
    Ui::SaveResultTester *ui;
};

#endif // SAVERESULTTESTER_H
