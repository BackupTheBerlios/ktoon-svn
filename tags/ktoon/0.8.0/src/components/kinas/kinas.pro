# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/kinas
# Target is a library:  

QT += xml 
KDEV_QTVER = 4 
INCLUDEPATH += ../../../src/ktoonlib \
               ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += ktseditor.h \
           ktsfunctionview.h \
           kinaswidget.h 
SOURCES += ktseditor.cpp \
           ktsfunctionview.cpp \
           kinaswidget.cpp 