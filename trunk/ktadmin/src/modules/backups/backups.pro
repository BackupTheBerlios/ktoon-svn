TEMPLATE = lib

CONFIG += staticlib \
warn_on
HEADERS += modulewidget.h \
backuplistparser.h \
addbackupparser.h
SOURCES += modulewidget.cpp \
backuplistparser.cpp \
addbackupparser.cpp
include(../modules_config.pri)

