SOURCES = filterproxy.cpp

QT += network

CONFIG += c++11

HEADERS += \
    filterproxy.h

OTHER_FILES += \
    libssl.a

unix:  LIBS += -lssl
win32: LIBS += "C:\Users\ce-nicolas.brochard\Desktop\Perso\Formation\Network security\GIT\filterproxy\libssl.a"
#.\libssl.a

FILTER_FILES = block.txt transform.txt

unix {
    for(FILE,FILTER_FILES){
        QMAKE_POST_LINK += $$quote(cp $${PWD}/$${FILE} $${OUT_PWD}$$escape_expand(\n\t))
    }
}

win32 {
    for(FILE,FILTER_FILES){
        QMAKE_POST_LINK +=$$quote(cmd /c copy /y $${PWD}\$${FILE} $${OUT_PWD}$$escape_expand(\n\t))
    }
}
