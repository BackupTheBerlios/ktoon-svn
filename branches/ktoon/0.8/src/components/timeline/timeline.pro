# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/timeline
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
          qt \
          staticlib 
TEMPLATE = lib 
HEADERS += kttimeline.h \
           ktlayermanager.h \
           ktlayersequence.h \
           kttimelinelayer.h \
           kttlruler.h \
           ktframesequencecontainer.h \
           tframestable.h 
SOURCES += kttimeline.cpp \
           ktlayermanager.cpp \
           ktlayersequence.cpp \
           kttimelinelayer.cpp \
           kttlruler.cpp \
           ktframesequencecontainer.cpp \
           tframestable.cpp 
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
