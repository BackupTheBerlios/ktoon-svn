# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/pen
# Target is a library:  

QT += xml
win32 {
   LIBS += -ldcore \
   -ldgui \
   -lstore \
   -lktoon
   QMAKE_LIBDIR = ../../../src/dlib/dcore/release \
   ../../../src/dlib/dgui/release \
   ../../../src/ktoonlib/release \
   ../../../src/store/release
}
KDEV_QTVER = 4 
INCLUDEPATH += ../../../src/store \
               ../../../src/ktoonlib \
               ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
	  staticlib
TEMPLATE = lib 
HEADERS += ktpenwidget.h 
SOURCES += ktpenwidget.cpp 

