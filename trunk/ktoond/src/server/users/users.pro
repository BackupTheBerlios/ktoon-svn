TEMPLATE = lib

CONFIG += staticlib \
warn_on

LIBDIR = "../lib"
include($$LIBDIR/lib.pri)

HEADERS += parser.h \
user.h \
manager.h
SOURCES += parser.cpp \
user.cpp \
manager.cpp
include(../../ktoond_config.pri)

