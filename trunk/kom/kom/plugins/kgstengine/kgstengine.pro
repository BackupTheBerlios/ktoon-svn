# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/kom/plugins/kgstengine
# Target is a library:  

INSTALLS += target 
target.path = /lib/kom/plugins/ 
INCLUDEPATH += ../../ 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 

!include(../../komconfig.pri){
    error("Please configure first")
}

contains(DEFINES, HAVE_GST10){
    HEADERS += kgstengine.h 
    SOURCES += kgstengine.cpp 
}


