# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/packages
# Target is a library:  

HEADERS += adduser.h \
           connect.h \
           ackparser.h \
           addban.h \
           removeban.h \
           removeuser.h \
           updateuser.h \
           queryuser.h \
           addproject.h \
           projectlistparser.h \
           listprojects.h \
           addbackup.h \
           removebackup.h \
           restorebackup.h \
           removeproject.h \
           updateproject.h \
           queryproject.h \
           userlistparser.h \
           wall.h
SOURCES += adduser.cpp \
           connect.cpp \
           ackparser.cpp \
           addban.cpp \
           removeban.cpp \
           removeuser.cpp \
           updateuser.cpp \
           queryuser.cpp \
           addproject.cpp \
           projectlistparser.cpp \
           listprojects.cpp \
           addbackup.cpp \
           removebackup.cpp \
           restorebackup.cpp \
           removeproject.cpp \
           updateproject.cpp \
           queryproject.cpp \
           userlistparser.cpp \
           wall.cpp
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
include(packages_config.pri)
