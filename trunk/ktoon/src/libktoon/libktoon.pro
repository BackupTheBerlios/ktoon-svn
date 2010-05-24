# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/libktoon
# Target is a library:  ktoon

INSTALLS += headers \
            target 
target.path = /lib/ 
headers.files += *.h 
headers.path = /include/ktoon 
HEADERS += ktgraphicalgorithm.h \
           ktglobal.h \
           ktpaintareaproperties.h \
           ktgradientcreator.h \
           ktgradientselector.h \
           ktgradientviewer.h \
           ktpackagehandler.h \
           ktprojectactionbar.h \
           ktbrushmanager.h \
           ktinputdeviceinformation.h \
           ktsvg2qt.h \
           ktitempreview.h \
           ktcompress.h  \
           ktsocketbase.h \
           ktxmlparserbase.h \
           ktproxyitem.h \
           ktinthash.h
SOURCES += ktgraphicalgorithm.cpp \
           ktpaintareaproperties.cpp \
           ktgradientcreator.cpp \
           ktgradientselector.cpp \
           ktgradientviewer.cpp \
           ktpackagehandler.cpp \
           ktprojectactionbar.cpp \
           ktbrushmanager.cpp \
           ktinputdeviceinformation.cpp \
           ktsvg2qt.cpp \
           ktitempreview.cpp \
           ktcompress.cpp  \
           ktsocketbase.cpp \
           ktxmlparserbase.cpp \
           ktproxyitem.cpp

#KLIB_DIR = "../../src/klib"
#include($$KLIB_DIR/klib.pri)

QUAZIP_DIR = "../../3rdparty/quazip/"

include($$QUAZIP_DIR/quazip.pri)

TARGET = ktoon

CONFIG += dll
TEMPLATE = lib
include(../../ktconfig.pri)


