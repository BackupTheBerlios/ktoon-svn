# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/export/ffmpegplugin
# Target is a library:  

INSTALLS += target 
target.path = /plugins/ 
KDEV_QTVER = 4 


include(../export_config.pri)

CONFIG += plugin 

TEMPLATE = lib 

HEADERS += ffmpegplugin.h
SOURCES += ffmpegplugin.cpp
!include(../../../../ktconfig.pri){
    error("Please run configure first")
}
