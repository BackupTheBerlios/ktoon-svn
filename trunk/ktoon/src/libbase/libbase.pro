# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/libbase
# Target is a library:  ktbase

INSTALLS += headers \
            target 
target.path = /lib/ 
headers.files += *.h 
headers.path = /include/base 
HEADERS += ktexportinterface.h \
           ktexportpluginobject.h \
           ktfilterinterface.h \
           ktmodulewidgetbase.h \
           ktpluginmanager.h \
           kttoolinterface.h \
           kttoolplugin.h  \
           ktpaintareabase.h \
           ktpaintarearotator.h \
           ktgraphicsscene.h \
           ktguideline.h \
           ktanimationrenderer.h
SOURCES += ktexportpluginobject.cpp \
           ktmodulewidgetbase.cpp \
           ktpluginmanager.cpp \
           kttoolplugin.cpp  \
           ktpaintareabase.cpp \
           ktpaintarearotator.cpp \
           ktgraphicsscene.cpp \
           ktguideline.cpp \
           ktanimationrenderer.cpp

#KLIB_DIR = ../../src/klib
#include($$KLIB_DIR/klib.pri)

QUAZIP_DIR = ../../3rdparty/quazip/
include($$QUAZIP_DIR/quazip.pri)
LIBKTOON_DIR = ../../src/libktoon
include($$LIBKTOON_DIR/libktoon.pri)
STORE_DIR = ../../src/store
include($$STORE_DIR/store.pri)
TARGET = ktbase
CONFIG += dll
TEMPLATE = lib
include(../../ktconfig.pri)
