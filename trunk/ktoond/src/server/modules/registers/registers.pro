TEMPLATE = lib

CONFIG += staticlib \
warn_on


HEADERS += manager.h \
actionregisteruserparser.h \
database.h \
adduser.h
SOURCES += manager.cpp \
actionregisteruserparser.cpp \
database.cpp \
adduser.cpp
include(../modules_config.pri)

