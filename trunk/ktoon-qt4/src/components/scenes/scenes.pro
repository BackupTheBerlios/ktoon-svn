# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/scenes
# Target is a library:  

HEADERS += ktsceneslist.h \
           ktsceneswidget.h 
SOURCES += ktsceneslist.cpp \
           ktsceneswidget.cpp 
INCLUDEPATH += ../../../src/dialogs \
../../../src/store \
../../../src/lib \
../../../src/qt/cameras \
../../../src/qt/tools \
../../../src/qt/draw \
../../../src/opengl/draw \
../../../src/opengl/tools \
../../../src/opengl/cameras
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
QT += qt3support xml opengl
