SOURCES = filterproxy.cpp

QT += network
RESOURCES += filterproxy.qrc

HEADERS += \
    filterproxy.h

OTHER_FILES += \
    libssl.a

unix|win32: LIBS += "C:\Users\ce-nicolas.brochard\Desktop\Perso\Formation\Network security\GIT\filterproxy\libssl.a"
#.\libssl.a
