# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/dlib/dsound/dgstengine
# Target is a library:  

include(../../dlibconf.pri) {
error("Please configure first")
}

INSTALLS += target 
target.path = /plugins/ 
KDEV_QTVER = 4 
INCLUDEPATH += ../player 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 

contains(DEFINES, HAVE_GST10) {
HEADERS += dgstengine.h 
SOURCES += dgstengine.cpp 
}

