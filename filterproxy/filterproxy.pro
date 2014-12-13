SOURCES = filterproxy.cpp
QT += network
RESOURCES += filterproxy.qrc

HEADERS += \
    filterproxy.h

unix|win32: LIBS += -lssl

OTHER_FILES +=
