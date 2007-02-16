TEMPLATE = lib

CONFIG += staticlib \
warn_on

include(../modules_config.pri)
HEADERS += modulewidget.h \
registerlistparser.h
SOURCES += modulewidget.cpp \
registerlistparser.cpp
