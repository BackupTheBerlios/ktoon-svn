# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/server/packages
# Target is a library:  serverpackages

HEADERS += connectparser.h \
           chatparser.h \
           noticeparser.h \
           wallparser.h \
           ack.h \
           error.h \
           banlist.h \
           useractionparser.h \
           userlist.h \
           removebanparser.h
	   
SOURCES += connectparser.cpp \
           chatparser.cpp \
           noticeparser.cpp \
           wallparser.cpp \
           ack.cpp \
           error.cpp \
           banlist.cpp \
           useractionparser.cpp \
           userlist.cpp \
           removebanparser.cpp
	   
TEMPLATE = lib
CONFIG += staticlib \
warn_on
TARGET = serverpackages
TARGET = serverpackages
!include(../../../ktoond_config.pri){
    error("Configure first")
}
