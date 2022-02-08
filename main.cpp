#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << QStyleFactory::keys();
    MainWindow w;
    QSettings settings( "settings_demo.conf", QSettings::IniFormat );
    w.show();
    return a.exec();
}
