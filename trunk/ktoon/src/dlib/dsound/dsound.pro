# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/dsound
# Target is a subdirs project 

INSTALLS += target 
target.path = /lib/ 
SUBDIRS += player \
           dgstengine 
QT += xml 
KDEV_QTVER = 4 
INCLUDEPATH += ../src 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          dll 
TEMPLATE = subdirs 
