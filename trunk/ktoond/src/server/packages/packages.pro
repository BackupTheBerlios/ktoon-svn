TEMPLATE = lib

CONFIG += staticlib \
warn_on

!include(../../../ktoond_config.pri){
    error("Configure first")
}


SOURCES += error.cpp connectparser.cpp \
chatparser.cpp \
noticeparser.cpp \
wallparser.cpp
HEADERS += error.h connectparser.h \
chatparser.h \
noticeparser.h \
wallparser.h
