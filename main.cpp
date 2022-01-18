#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QSettings settings( "settings_demo.conf", QSettings::IniFormat );
    w.show();
    return a.exec();
}
