QT       += core gui
QT      += serialport
QT      += axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addstepwindow.cpp \
    channelnamemodel.cpp \
    devicetester.cpp \
    deviceu2270.cpp \
    main.cpp \
    mainwindow.cpp \
    reportclass.cpp \
    safetester.cpp \
    saveresulttester.cpp \
    serialportwindow.cpp \
    tableviewconfigmodel.cpp \
    testthread.cpp \
    u2270.cpp \
    worker.cpp

HEADERS += \
    addstepwindow.h \
    channelnamemodel.h \
    devicetester.h \
    deviceu2270.h \
    mainwindow.h \
    reportclass.h \
    safetester.h \
    saveresulttester.h \
    serialportwindow.h \
    settingsDefine.h \
    tableviewconfigmodel.h \
    testthread.h \
    u2270.h \
    worker.h

FORMS += \
    addstepwindow.ui \
    mainwindow.ui \
    saveresulttester.ui \
    serialportwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    interface.txt \
    style \
    u2270_serial

RESOURCES += \
    resource.qrc

RC_ICONS = img/icon.ico

