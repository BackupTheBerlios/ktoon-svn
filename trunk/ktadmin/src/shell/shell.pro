TEMPLATE = app

CONFIG += qt \
warn_on
QT += core \
gui \
xml \
network
SOURCES += main.cpp \
mainwindow.cpp \
manager.cpp \
socket.cpp
HEADERS += mainwindow.h \
manager.h \
socket.h
TARGET = ktadmin

DESTDIR = ../../bin


include(../../ktadmin_config.pri)

MODULES_DIR = ../modules
include($$MODULES_DIR/modules.pri)

LIB_DIR = ../lib
include($$LIB_DIR/lib.pri)
