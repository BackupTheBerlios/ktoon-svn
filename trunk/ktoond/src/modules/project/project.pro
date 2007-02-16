TEMPLATE = lib

CONFIG += staticlib \
warn_on
SOURCES += database.cpp \
databaseparser.cpp \
projectcollection.cpp \
sproject.cpp
HEADERS += database.h \
databaseparser.h \
projectcollection.h \
sproject.h

!include(../modules_config.pri){
    error("projects")
}

