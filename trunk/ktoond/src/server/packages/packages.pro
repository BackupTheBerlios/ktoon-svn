TEMPLATE = lib

CONFIG += staticlib \
warn_on

!include(../../../ktoond_config.pri) {
error("Configure first")
}


SOURCES += error.cpp userpackageparser.cpp

HEADERS += error.h userpackageparser.h

