# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/tools/select
# Target is a library:  

QT += xml 
INSTALLS += target 
target.path = /plugins/ 
KDEV_QTVER = 4 
LIBS += -lstore \
        -ldsound \
        -ldcore 
INCLUDEPATH += ../../../../src/store \
               ../../../../src/libktoon \
               ../../../../src/dlib/dgui \
               ../../../../src/dlib/dsound \
               ../../../../src/dlib/dcore 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
QMAKE_LIBDIR = ../../../../src/store \
               ../../../../src/dlib/dsound \
               ../../../../src/dlib/dcore 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 
HEADERS += select.h \
           node.h 
SOURCES += select.cpp \
           node.cpp 
