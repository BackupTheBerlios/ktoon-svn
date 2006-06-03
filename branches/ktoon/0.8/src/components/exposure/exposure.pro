# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/exposure
# Target is a library:  

QT += xml 
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
HEADERS += esframe.h \
           eslayer.h \
           ktexposuresheet.h \
           kttableexposure.h \
           ktlayerexposure.h 
SOURCES += esframe.cpp \
           eslayer.cpp \
           ktexposuresheet.cpp \
           kttableexposure.cpp \
           ktlayerexposure.cpp 
