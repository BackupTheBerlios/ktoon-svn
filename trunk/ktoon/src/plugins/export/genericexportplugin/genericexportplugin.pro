# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/export/genericexportplugin
# Target is a library:  

QT += xml 
KDEV_QTVER += 4 
INSTALLS += target 
target.path = /plugins/ 
INCLUDEPATH += ../../../../src/libktoon \
               ../../../../src/store \
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

