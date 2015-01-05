#-------------------------------------------------
#
# Project created by QtCreator 2014-12-11T16:48:22
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ui-Netwrok
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    options.cpp \
    packets.cpp \
    addoption.cpp \
    ../libfilterproxy/filterproxy.cpp

HEADERS  += \
    mainwindow.h \
    options.h \
    packets.h \
    addoption.h \
    ../libfilterproxy/filterproxy.h

FORMS    += \
    mainwindow.ui \
    options.ui \
    packets.ui \
    addoption.ui

RESOURCES += \
    img/ui.qrc

INCLUDEPATH +=  ../libfilterproxy
