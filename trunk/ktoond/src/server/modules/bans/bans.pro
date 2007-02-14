TEMPLATE = lib

CONFIG += staticlib \
warn_on
SOURCES += banmanager.cpp \
addbanparser.cpp \
banlist.cpp \
removebanparser.cpp
HEADERS += banmanager.h \
addbanparser.h \
banlist.h \
removebanparser.h
include(../modules_config.pri)
