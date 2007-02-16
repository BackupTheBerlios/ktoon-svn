TEMPLATE = lib

CONFIG += staticlib \
warn_on

include(../modules_config.pri)
HEADERS += modulewidget.h \
registerlistparser.h \
registeruser.h \
actionregisteruserparser.h
SOURCES += modulewidget.cpp \
registerlistparser.cpp \
registeruser.cpp \
actionregisteruserparser.cpp
