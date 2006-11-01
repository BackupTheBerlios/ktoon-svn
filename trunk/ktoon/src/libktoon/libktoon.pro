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
           ktgraphicelement.h \
           ktpackagehandler.h \
           ktprojectactionbar.h \
           ktbrushmanager.h \
           ktinputdeviceinformation.h \
           ktsvg2qt.h \
           ktitempreview.h \
           ktcompress.h 
SOURCES += ktgraphicalgorithm.cpp \
           ktpaintareaproperties.cpp \
           ktgradientcreator.cpp \
           ktgradientselector.cpp \
           ktgradientviewer.cpp \
           ktgraphicelement.cpp \
           ktpackagehandler.cpp \
           ktprojectactionbar.cpp \
           ktbrushmanager.cpp \
           ktinputdeviceinformation.cpp \
           ktsvg2qt.cpp \
           ktitempreview.cpp \
           ktcompress.cpp 
DLIB_DIR = ../../src/dlib
include($$DLIB_DIR/dlib.pri)
QUAZIP_DIR = ../../3rdparty/quazip/
include($$QUAZIP_DIR/quazip.pri)
KDEV_QTVER = 4
TARGET = ktoon
CONFIG += dll
TEMPLATE = lib
include(../../ktconfig.pri)
