# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/modules/arthury/plugins
# Target is a subdirs project 

SUBDIRS += tools \
           filters 
QT += xml 
TARGETDEPS += ../../../../src/lib/liblib.a 
LIBS += ../../../../src/lib/liblib.a 
INCLUDEPATH += ../../../../src/modules/arthury/interfaces \
               ../../../../src/lib 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
TARGET = arthuryplugins 
CONFIG += release \
          warn_on 
TEMPLATE = subdirs 
