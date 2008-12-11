# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./kom/kcore
# Target is a library:  

INSTALLS += include \
            target 

target.path = /lib/ 
include.files += *.h 

include.path = /include/kcore 

HEADERS += kalgorithm.h \
           kapplicationproperties.h \
           kaspellchecker.h \
           kconfig.h \
           kcpuinfo.h \
           kdebug.h \
           kfortunegenerator.h \
           kglobal.h \
           kmd5hash.h \
           kspeller.h \
           kspellhighlighter.h \
           kspellinterface.h \
           ktipdatabase.h \
           kxmlparserbase.h
SOURCES += kalgorithm.cpp \
           kapplicationproperties.cpp \
           kaspellchecker.cpp \
           kconfig.cpp \
           kcpuinfo.cpp \
           kdebug.cpp \
           kfortunegenerator.cpp \
           kmd5hash.cpp \
           kspeller.cpp \
           kspellhighlighter.cpp \
           ktipdatabase.cpp \
           kxmlparserbase.cpp

DEFINES += KLIB_CORE
INCLUDEPATH += ../
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release warn_on dll create_prl
TEMPLATE = lib
QMAKE_STRIP = echo

!include(../komconfig.pri){
    error("Run ./configure first")
}

