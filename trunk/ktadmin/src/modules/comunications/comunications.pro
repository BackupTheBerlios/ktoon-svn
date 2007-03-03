TEMPLATE = lib

CONFIG += staticlib \
warn_on

include(../modules_config.pri)


SOURCES += walldialog.cpp \
observer.cpp
HEADERS += walldialog.h \
observer.h
