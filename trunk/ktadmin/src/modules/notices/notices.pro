TEMPLATE = lib

CONFIG += staticlib \
warn_on

include(../modules_config.pri)

HEADERS += noticedialog.h \
noticeparser.h \
noticeobserver.h
SOURCES += noticedialog.cpp \
noticeparser.cpp \
noticeobserver.cpp
