# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/dlib/plugins/dgstengine
# Target is a library:  

INSTALLS += target 
target.path = /plugins/ 
KDEV_QTVER = 4 
INCLUDEPATH += ../../ 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 
! include(../../dlibconfig.pri) {
error("Please configure first")
}


contains(DEFINES, HAVE_GST10) {
HEADERS += dgstengine.h 
SOURCES += dgstengine.cpp 
}

