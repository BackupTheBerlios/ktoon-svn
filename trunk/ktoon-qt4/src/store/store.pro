# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/store
# Target is a library:  

INSTALLS += headers \
            target 
target.path = /lib/ 
headers.files += *.h 
headers.path = /include/ 
HEADERS += ktkeyframe.h \
           agraphiccomponent.h \
           ktlayer.h \
           ktprojectmanager.h \
           ktscene.h \
           ktdocument.h \
           ktserializableobject.h \
           ktbrush.h \
           ktprojectparser.h \
           ktpalettedocument.h 
SOURCES += ktkeyframe.cpp \
           agraphiccomponent.cpp \
           ktlayer.cpp \
           ktprojectmanager.cpp \
           ktscene.cpp \
           ktdocument.cpp \
           ktserializableobject.cpp \
           ktbrush.cpp \
           ktprojectparser.cpp \
           ktpalettedocument.cpp 
QT += xml
KDEV_QTVER = 4
INCLUDEPATH += ../../src/ktoonlib \
../../src/core \
../../src/dialogs \
../../src/store \
../../src/dlib/dgui \
../../src/dlib/dcore
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release \
warn_on \
dll
TEMPLATE = lib
