# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/net
# Target is a library:  ktnet

HEADERS += ktnetprojectmanagerparams.h \
           ktnetprojectmanagerhandler.h \
           ktnetsocket.h \
           ktconnectpackage.h \
           ktlistpackage.h \
           ktopenpackage.h \
           ktnewprojectpackage.h \
           ktsavenetproject.h \
           ktchat.h \
           ktconnectdialog.h \
           ktprojectparser.h  \
           ktlistprojectspackage.h
SOURCES += ktnetprojectmanagerparams.cpp \
           ktnetprojectmanagerhandler.cpp \
           ktnetsocket.cpp \
           ktconnectpackage.cpp \
           ktlistpackage.cpp \
           ktopenpackage.cpp \
           ktnewprojectpackage.cpp \
           ktsavenetproject.cpp \
           ktchat.cpp \
           ktconnectdialog.cpp \
           ktprojectparser.cpp  \
           ktlistprojectspackage.cpp
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
include(net_config.pri)
TARGET = ktnet
include(../../ktconfig.pri)
