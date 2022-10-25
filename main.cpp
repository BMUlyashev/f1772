#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>
#include <QDebug>
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << QStyleFactory::keys();

//    QFile styleSheetFile("./Combinear.qss");
//    styleSheetFile.open(QFile::ReadOnly);
//    QString styleSheet = QLatin1String(styleSheetFile.readAll());
//    a.setStyleSheet(":/style/style");
//    styleSheetFile.close();
    MainWindow w;
    QSettings settings( "settings_demo.conf", QSettings::IniFormat );
    w.show();
    return a.exec();
}
