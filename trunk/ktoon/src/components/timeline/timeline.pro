# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/timeline
# Target is a library:  

HEADERS += kttimeline.h \
           ktlayermanager.h \
           kttlruler.h \
           ktframestable.h 
SOURCES += kttimeline.cpp \
           ktlayermanager.cpp \
           kttlruler.cpp \
           ktframestable.cpp 
QT += xml
KDEV_QTVER = 4
INCLUDEPATH += ../../../src/store \
../../../src/libktoon \
../../../src/dlib/dgui \
../../../src/dlib/dcore \
../../../src/dlib
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release \
warn_on \
qt \
staticlib
TEMPLATE = lib