# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/modules/arthury/plugins/tools/afilltool
# Target is a library:  

QT += xml 
INSTALLS += target 
target.path = /plugins/ 
KDEV_QTVER = 4 
LIBS += -lstore \
        -lktoon \
        -ldgui \
        -ldcore 
INCLUDEPATH += ../../../../../../src/modules/arthury/interfaces \
               ../../../../../../src/store \
               ../../../../../../src/interfaces \
               ../../../../../../src/ktoonlib \
               ../../../../../../src/dlib/dgui \
               ../../../../../../src/dlib/dcore 
QMAKE_LIBDIR = ../../../../../../src/store \
               ../../../../../../src/ktoonlib \
               ../../../../../../src/dlib/dgui \
               ../../../../../../src/dlib/dcore 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 
HEADERS += afilltool.h 
SOURCES += afilltool.cpp 
win32 {
  QMAKE_LIBDIR = ../../../../../../src/dlib/dcore/release \
  ../../../../../../src/dlib/dgui/release \
  ../../../../../../src/ktoonlib/release \
  ../../../../../../src/store/release
}
