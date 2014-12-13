#-------------------------------------------------
#
# Project created by QtCreator 2014-12-11T16:48:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ui-Netwrok
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    options.cpp \
    packets.cpp \
    addoption.cpp

HEADERS  += \
    mainwindow.h \
    options.h \
    packets.h \
    addoption.h

FORMS    += \
    mainwindow.ui \
    options.ui \
    packets.ui \
    addoption.ui

RESOURCES += \
    img/ui.qrc
