# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/paintarea
# Target is a library:  paintarea

HEADERS += ktpaintarea.h \
           ktconfigurationarea.h \
           ktviewdocument.h \
           ktdocumentruler.h \
           ktpaintareastatus.h \
           ktimagedevice.h \
           ktpaintareacommand.h 
SOURCES += ktpaintarea.cpp \
           ktconfigurationarea.cpp \
           ktviewdocument.cpp \
           ktdocumentruler.cpp \
           ktpaintareastatus.cpp \
           ktimagedevice.cpp \
           ktpaintareacommand.cpp 
KDEV_QTVER = 4

STORE_DIR = ../../../src/store
LIBKTOON_DIR = ../../../src/libktoon
DLIB_DIR = ../../../src/dlib 

include($$STORE_DIR/store.pri)
include($$LIBKTOON_DIR/libktoon.pri)
include($$DLIB_DIR/dlib.pri)

MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
QT += xml opengl
TARGET = paintarea
