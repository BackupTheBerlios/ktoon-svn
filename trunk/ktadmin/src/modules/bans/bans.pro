TEMPLATE = lib

CONFIG += staticlib \
warn_on

include(../modules_config.pri)

HEADERS += modulewidget.h \
removebanparser.h \
banlistparser.h \
form.h \
addbanparser.h
SOURCES += modulewidget.cpp \
removebanparser.cpp \
banlistparser.cpp \
form.cpp \
addbanparser.cpp
