# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/export/genericexportplugin
# Target is a library:  

QT += xml 
INSTALLS += target 
target.path = /plugins/ 
KDEV_QTVER = 4 
INCLUDEPATH += ../../../../src/store \
               ../../../../src/ktoonlib \
               ../../../../src/dlib/dgui \
               ../../../../src/dlib/dcore 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 
HEADERS += genericexportplugin.h 
SOURCES += genericexportplugin.cpp 
macx{
  LIBS += -ldcore -ldgui -lktoon -lstore
  QMAKE_LIBDIR = ../../../../src/dlib/dcore ../../../../src/dlib/dgui ../../../../src/ktoonlib ../../../../src/store
}
