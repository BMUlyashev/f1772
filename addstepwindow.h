#ifndef ADDSTEPWINDOW_H
#define ADDSTEPWINDOW_H

#include <QDialog>

namespace Ui {
class AddStepWindow;
}

class AddStepWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddStepWindow(QWidget *parent = nullptr);
    ~AddStepWindow();

    int getCountValue();
    void setCountValue(int value);


private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddStepWindow *ui;
    int countValue;
};

#endif // ADDSTEPWINDOW_H
