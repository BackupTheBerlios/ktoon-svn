TEMPLATE = lib

CONFIG += staticlib \
warn_on

HEADERS += parser.h \
user.h \
manager.h \
database.h
SOURCES += parser.cpp \
user.cpp \
manager.cpp \
database.cpp
include(../../../ktoond_config.pri)

