TEMPLATE = lib

SOURCES = filterproxy.cpp \
    filterproxyapp.cpp

QT += network

CONFIG += c++11

HEADERS += \
    filterproxy.h \
    filterproxyapp.h

OTHER_FILES += \
    libssl.a

unix:  LIBS += -lssl
win32: LIBS += "C:\Users\ce-nicolas.brochard\Desktop\Perso\Formation\Network security\GIT\filterproxy\libssl.a"
#.\libssl.a

TARGET  = filterproxy
DESTDIR = ../lib
