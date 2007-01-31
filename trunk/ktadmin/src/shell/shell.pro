TEMPLATE = app

CONFIG += qt \
warn_on
QT += core \
gui \
xml \
network
SOURCES += main.cpp \
mainwindow.cpp
HEADERS += mainwindow.h

TARGET = ktadmin

DESTDIR = ../../bin


include(../../ktadmin_config.pri)

MODULES_DIR = ../modules
include($$MODULES_DIR/modules.pri)

