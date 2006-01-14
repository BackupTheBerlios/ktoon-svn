# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/dialogs
# Target is a library:  

QT += qt3support xml 
KDEV_QTVER = 4 
INCLUDEPATH += ../../src/core \
               ../../src/store \
               ../../src/lib \
               ../../src/modules/opengl/draw \
               ../../src/modules/opengl/tools \
               ../../src/modules/opengl/cameras \
               ../../src/images/images 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += preferences.h \
           ktthemeselector.h \
           ktabout.h 
SOURCES += preferences.cpp \
           ktthemeselector.cpp \
           ktabout.cpp 
