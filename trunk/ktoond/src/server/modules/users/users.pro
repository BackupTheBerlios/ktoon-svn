TEMPLATE = lib

CONFIG += staticlib \
warn_on

HEADERS += parser.h \
user.h \
manager.h \
database.h \
right.h
SOURCES += parser.cpp \
user.cpp \
manager.cpp \
database.cpp \
right.cpp

include(../modules_config.pri)

