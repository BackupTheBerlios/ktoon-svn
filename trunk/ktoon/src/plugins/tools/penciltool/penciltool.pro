# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/tools/penciltool
# Target is a library:  

INSTALLS += target 
target.path = /plugins/
KDEV_QTVER = 4 

include(../tools_config.pri)

MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += plugin 
include(../../../../ktconfig.pri)
TEMPLATE = lib 
HEADERS += brush.h \
           exactnessconfigurator.h 
SOURCES += brush.cpp \
           exactnessconfigurator.cpp 

