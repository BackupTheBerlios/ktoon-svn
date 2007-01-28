# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/shell
# Target is an application:  ../../bin/ktoond.bin

HEADERS += projectcollection.h \
           packagehandler.h \
           database.h \
           databaseparse.h  \
           sproject.h
SOURCES += main.cpp \
           projectcollection.cpp \
           packagehandler.cpp \
           database.cpp \
           databaseparse.cpp  \
           sproject.cpp
TARGET = ../../bin/ktoond.bin
CONFIG += release \
warn_on
TEMPLATE = app
include(shell_config.pri)
LIB_DIR = ../lib
include($$LIB_DIR/lib.pri)
SERVER_DIR = ../server
include($$SERVER_DIR/server.pri)
PACKAGES_DIR = ../packages/
include($$PACKAGES_DIR/packages.pri)
