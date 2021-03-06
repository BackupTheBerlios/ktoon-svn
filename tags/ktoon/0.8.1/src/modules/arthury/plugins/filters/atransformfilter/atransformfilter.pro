# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/modules/arthury/plugins/filters/atransformfilter
# Target is a library:  

QT += xml 
INSTALLS += atransformfilter \
            target 
target.path = /plugins/ 
atransformfilter.files += *.so 
atransformfilter.path = /plugins/ 
KDEV_QTVER = 4 
INCLUDEPATH += ../../../../../../src/modules/arthury/interfaces \
               ../../../../../../src/store \
               ../../../../../../src/ktoonlib \
               ../../../../../../src/dlib/dgui \
               ../../../../../../src/dlib/dcore 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 
HEADERS += atransformfilter.h 
SOURCES += atransformfilter.cpp 


LIBS += -lstore \
        -lktoon \
        -ldgui \
        -ldcore 

QMAKE_LIBDIR = ../../../../../../src/store \
               ../../../../../../src/ktoonlib \
               ../../../../../../src/dlib/dgui \
               ../../../../../../src/dlib/dcore
win32 {
  QMAKE_LIBDIR = ../../../../../../src/dlib/dcore/release \
  ../../../../../../src/dlib/dgui/release \
  ../../../../../../src/ktoonlib/release \
  ../../../../../../src/store/release
}

