TEMPLATE = lib

QT += network
CONFIG += staticlib \
warn_on
SOURCES += connection.cpp \
defaultpackagehandler.cpp \
packagehandlerbase.cpp \
server.cpp \
serverclient.cpp
HEADERS += connection.h \
defaultpackagehandler.h \
packagehandlerbase.h \
server.h \
serverclient.h


!include(../../../ktoond_config.pri) {
error("configure first")
}
include(../server_config.pri)

TARGET = server

