# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/help
# Target is a library:  

QT += xml 
INSTALLS += help 
help.files += help 
help.path = /data/ 
KDEV_QTVER = 4 
INCLUDEPATH += ../../../src/ktoonlib \
               ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += kthelpwidget.h \
           kthelpbrowser.h 
SOURCES += kthelpwidget.cpp \
           kthelpbrowser.cpp 
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