TEMPLATE = lib

CONFIG += staticlib \
warn_on
HEADERS += modulewidget.h \
backuplistparser.h
SOURCES += modulewidget.cpp \
backuplistparser.cpp
include(../modules_config.pri)

