SOURCES = main.cpp

QT += network

CONFIG += c++11

OTHER_FILES += \
    libssl.a

unix:  LIBS += -lssl
win32: LIBS += "C:\Users\ce-nicolas.brochard\Desktop\Perso\Formation\Network security\GIT\filterproxy\libssl.a"

unix:  LIBS += ../lib/libfilterproxy.so
win32: LIBS += ../lib/libfilterproxy.dll

#.\libssl.a

FILTER_FILES = block.txt transform.txt

for(FILE,FILTER_FILES) {
  unix:  QMAKE_POST_LINK += $$quote(cp $${PWD}/$${FILE} $${OUT_PWD}$$escape_expand(\n\t))
  win32: QMAKE_POST_LINK += $$quote(cmd /c copy /y $${PWD}\$${FILE} $${OUT_PWD}$$escape_expand(\n\t))
}
