TEMPLATE = lib

CONFIG += staticlib \
warn_on

!include(../../../ktoond_config.pri){
    error("Configure first")
}


SOURCES += connectparser.cpp \
chatparser.cpp \
noticeparser.cpp \
wallparser.cpp \
ack.cpp \
error.cpp \
banlist.cpp
HEADERS += connectparser.h \
chatparser.h \
noticeparser.h \
wallparser.h \
ack.h \
error.h \
banlist.h
TARGET = serverpackages

