# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/export/smilexportplugin
# Target is a library:  

QT += xml 
INSTALLS += target 
target.path = /plugins/ 
KDEV_QTVER = 4 
INCLUDEPATH += ../../../../src/store \
               ../../../../src/libktoon \
               ../../../../src/dlib/dgui \
               ../../../../src/dlib/dcore \
               ../../../../src/dlib 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 
HEADERS += smilexportplugin.h 
SOURCES += smilexportplugin.cpp 
win32{
  LIBS += -ldcore -ldgui -lktoon -lstore
  QMAKE_LIBDIR = ../../../../src/dlib/dcore/release \
  ../../../../src/dlib/dgui/release \
  ../../../../src/ktoonlib/release \
  ../../../../src/store/release
}
macx{
  LIBS += -ldcore -ldgui -lktoon -lstore
  QMAKE_LIBDIR += ../../../../src/store \
  ../../../../src/ktoonlib \
  ../../../../src/dlib/dgui \
  ../../../../src/dlib/dcore
}