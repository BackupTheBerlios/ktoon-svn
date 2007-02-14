TEMPLATE = lib

CONFIG += staticlib \
warn_on

!include(../../../ktoond_config.pri){
    error("Configure first")
}

SOURCES += logger.cpp \
observer.cpp \
package.cpp \
settings.cpp
HEADERS += logger.h \
observer.h \
package.h \
settings.h
