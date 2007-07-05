TEMPLATE = lib

INSTALLS += target
target.path = /plugins/
KDEV_QTVER = 4

include(../tools_config.pri)
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj

CONFIG += plugin
include(../../../../ktconfig.pri)
HEADERS += polyline.h

SOURCES += polyline.cpp

