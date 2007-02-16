TEMPLATE = lib

CONFIG += staticlib \
warn_on

HEADERS += parser.h \
user.h \
manager.h \
database.h \
right.h \
useractionparser.h \
userlist.h \
ack.h \
connectparser.h
SOURCES += parser.cpp \
user.cpp \
manager.cpp \
database.cpp \
right.cpp \
useractionparser.cpp \
userlist.cpp \
ack.cpp \
connectparser.cpp
include(../modules_config.pri)

