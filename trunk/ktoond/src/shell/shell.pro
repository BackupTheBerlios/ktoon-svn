# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/shell
# Target is an application:  ../../bin/ktoond

HEADERS += ktserver.h \
           ktserverclient.h \
           ktserverconnection.h 
SOURCES += main.cpp \
           ktserver.cpp \
           ktserverclient.cpp \
           ktserverconnection.cpp 
TARGET = ../../bin/ktoond.bin 
CONFIG += release \
          warn_on 
TEMPLATE = app 

include("shell_config.pri")
