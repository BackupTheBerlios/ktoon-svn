# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/colorpalette
# Target is a library:  

QT += xml 
INSTALLS += palettes 
palettes.files += palettes/*.ktpl 
palettes.path = /data/palettes/ 
KDEV_QTVER = 4 
INCLUDEPATH += ../../../src/ktoonlib \
               ../../../src/store \
               ../../../src/dlib/dgui \
               ../../../src/dlib/dcore 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          staticlib 

TEMPLATE = lib 
HEADERS += ktcolorpicker.h \
           ktcolorpalette.h \
           ktvaluecolor.h \
           ktviewcolorcells.h \
           ktluminancepicker.h \
           ktdualcolorbutton.h \
           ktcellscolor.h \
           ktpaletteparser.h 
SOURCES += ktcolorpicker.cpp \
           ktcolorpalette.cpp \
           ktvaluecolor.cpp \
           ktviewcolorcells.cpp \
           ktluminancepicker.cpp \
           ktdualcolorbutton.cpp \
           ktcellscolor.cpp \
           ktpaletteparser.cpp 

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

