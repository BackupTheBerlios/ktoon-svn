# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/paintarea
# Target is a library:  

HEADERS += ktpaintarea.h \
           ktconfigurationarea.h \
           ktviewdocument.h \
           ktdocumentruler.h \
           ktpaintareastatus.h \
           ktimagedevice.h 
SOURCES += ktpaintarea.cpp \
           ktconfigurationarea.cpp \
           ktviewdocument.cpp \
           ktdocumentruler.cpp \
           ktpaintareastatus.cpp \
           ktimagedevice.cpp 
KDEV_QTVER = 4
INCLUDEPATH += ../../../src/store \
../../../src/libktoon \
../../../src/dlib/dgui \
../../../src/dlib/dsound \
../../../src/dlib/dcore \
../../../src/dlib/
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
QT += xml opengl
