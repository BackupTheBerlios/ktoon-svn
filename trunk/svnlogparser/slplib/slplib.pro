# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./slplib
# Target is a library:  

QT += xml 
KDEV_QTVER = 4 
LIBS += -lsvn_client-1 
INCLUDEPATH += /usr/include/apr-0/ \
               /usr/include/subversion-1/ 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += slpparser.h \
           slpentry.h \
           slpmanager.h \
           slpgenerator.h 
SOURCES += slpparser.cpp \
           slpentry.cpp \
           slpmanager.cpp \
           slpgenerator.cpp 
