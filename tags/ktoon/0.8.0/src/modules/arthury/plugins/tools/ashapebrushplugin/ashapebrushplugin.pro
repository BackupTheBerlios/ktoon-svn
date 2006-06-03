# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/modules/arthury/plugins/tools/ashapebrushplugin
# Target is a library:  

INSTALLS += ashapebrush \
            target 
target.path = /plugins 
ashapebrush.files += *.so 
ashapebrush.path = /plugins/ 
HEADERS += ashapebrushplugin.h \
           shapeconfigurator.h \
           ktbrusheditor.h \
           ktbrusheslist.h \
           ktbrushesparser.h \
           ktdisplaybrush.h 
SOURCES += ashapebrushplugin.cpp \
           shapeconfigurator.cpp \
           ktbrusheditor.cpp \
           ktbrusheslist.cpp \
           ktbrushesparser.cpp \
           ktdisplaybrush.cpp 
QT += xml 
KDEV_QTVER = 4 
LIBS += -lstore \
        -lktoon \
        -ldgui \
        -ldcore 
INCLUDEPATH += ../../../../../../src/modules/arthury/interfaces \
               ../../../../../../src/store \
               ../../../../../../src/ktoonlib \
               ../../../../../../src/dlib/dgui \
               ../../../../../../src/dlib/dcore 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
QMAKE_LIBDIR = ../../../../../../src/store \
               ../../../../../../src/ktoonlib \
               ../../../../../../src/dlib/dgui \
               ../../../../../../src/dlib/dcore 
CONFIG += release \
          warn_on \
          plugin 
TEMPLATE = lib 
