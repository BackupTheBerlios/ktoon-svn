# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/dlib/dcore
# Target is a library:  


! include(../dlibconfig.pri ) {
error("Run ./configure first")
}

DEFINES += DLIB_CORE
QT += xml 
INSTALLS += include \
            target 
target.path = /lib/ 
include.files += *.h 
include.path = /include/dcore 
KDEV_QTVER = 4 
INCLUDEPATH += ../ 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          dll 
TEMPLATE = lib 
HEADERS += dalgorithm.h \
           dbrushadjuster.h \
           dconfig.h \
           dconfigdocument.h \
           ddebug.h \
           dguiitem.h \
           dmd5hash.h \
           dapplicationproperties.h \
           dglobal.h \
           dgradientadjuster.h \
           dtipdatabase.h \
           daspellchecker.h \
           dspellinterface.h \
           dcpuinfo.h \
           dimageeffect.h \
           dspeller.h \
           dspellhighlighter.h \
           dpathhandler.h 
SOURCES += dalgorithm.cpp \
           dbrushadjuster.cpp \
           dconfig.cpp \
           dconfigdocument.cpp \
           ddebug.cpp \
           dgradientadjuster.cpp \
           dguiitem.cpp \
           dmd5hash.cpp \
           dapplicationproperties.cpp \
           dtipdatabase.cpp \
           daspellchecker.cpp \
           dcpuinfo.cpp \
           dimageeffect.cpp \
           dspeller.cpp \
           dspellhighlighter.cpp \
           dpathhandler.cpp 
