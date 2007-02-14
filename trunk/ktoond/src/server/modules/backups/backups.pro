TEMPLATE = lib

CONFIG += staticlib \
warn_on

SOURCES += backupdatabase.cpp \
backupmanager.cpp \
addbackup.cpp \
backuplist.cpp \
removebackupparser.cpp
HEADERS += backupdatabase.h \
backupmanager.h \
addbackup.h \
backuplist.h \
removebackupparser.h
include(../modules_config.pri)

