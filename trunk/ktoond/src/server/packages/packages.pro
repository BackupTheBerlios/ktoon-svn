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
error.cpp
HEADERS += connectparser.h \
chatparser.h \
noticeparser.h \
wallparser.h \
ack.h \
error.h
TARGET = serverpackages

