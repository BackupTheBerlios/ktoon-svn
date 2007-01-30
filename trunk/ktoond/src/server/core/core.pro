TEMPLATE = lib

QT += network
CONFIG += staticlib \
warn_on
SOURCES += connection.cpp \
packagehandlerbase.cpp \
server.cpp \
serverclient.cpp \
defaultpackagehandler.cpp \
settings.cpp
HEADERS += connection.h \
packagehandlerbase.h \
server.h \
serverclient.h \
defaultpackagehandler.h \
settings.h
!include(../../../ktoond_config.pri){
    error("configure first")
}
include(../server_config.pri)

TARGET = server

