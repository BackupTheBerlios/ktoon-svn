# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/export/ffmpegplugin
# Target is a library:  

QT += xml 
INSTALLS += target 
target.path = /plugins/ 
KDEV_QTVER = 4 
LIBS += -ldcore \
        -ldgui \
        -lktoon 
INCLUDEPATH += ../../../../src/store \
               ../../../../src/ktoonlib \
               ../../../../src/dlib/dgui \
               ../../../../src/dlib/dcore 
QMAKE_LIBDIR = ../../../../src/dlib/dcore \
               ../../../../src/dlib/dgui \
               ../../../../src/ktoonlib 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 
HEADERS += ffmpegplugin.h \
           ffmpegmanager.h 
SOURCES += ffmpegplugin.cpp \
           ffmpegmanager.cpp 

!include(../../../../ktconfig.pri) {
error("Please run configure first")
}

win32 {
QMAKE_LIBDIR = ../../../../src/dlib/dcore/release ../../../../src/dlib/dgui/release ../../../../src/ktoonlib/release
}

