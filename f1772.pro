QT       += core gui
QT      += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addstepwindow.cpp \
    devicetester.cpp \
    deviceu2270.cpp \
    main.cpp \
    mainwindow.cpp \
    safetester.cpp \
    serialportwindow.cpp \
    tableviewconfigmodel.cpp \
    testthread.cpp \
    u2270.cpp \
    worker.cpp

HEADERS += \
    addstepwindow.h \
    devicetester.h \
    deviceu2270.h \
    mainwindow.h \
    safetester.h \
    serialportwindow.h \
    settingsDefine.h \
    tableviewconfigmodel.h \
    testthread.h \
    u2270.h \
    worker.h

FORMS += \
    addstepwindow.ui \
    mainwindow.ui \
    serialportwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    interface.txt \
    u2270_serial

RESOURCES += \
    resource.qrc

