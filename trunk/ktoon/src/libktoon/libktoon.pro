# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/libktoon
# Target is a library:  ktoon

INSTALLS += target 
target.path = /lib/ 
HEADERS += ktmodulewidgetbase.h \
           ktmodulewidgettitle.h \
           ktexportpluginobject.h \
           ktgraphicalgorithm.h \
           ktglobal.h \
           ktpaintareaproperties.h \
           ktgradientcreator.h \
           ktgradientselector.h \
           ktgradientviewer.h \
           ktanimationarea.h \
           ktexportinterface.h \
           ktfilterinterface.h \
           ktgraphicelement.h \
           ktpackagehandler.h \
           kttoolinterface.h \
           ktprojectactionbar.h \
           kttoolplugin.h \
           ktbrushmanager.h 
SOURCES += ktmodulewidgetbase.cpp \
           ktmodulewidgettitle.cpp \
           ktexportpluginobject.cpp \
           ktgraphicalgorithm.cpp \
           ktpaintareaproperties.cpp \
           ktgradientcreator.cpp \
           ktgradientselector.cpp \
           ktgradientviewer.cpp \
           ktanimationarea.cpp \
           ktgraphicelement.cpp \
           ktpackagehandler.cpp \
           ktprojectactionbar.cpp \
           kttoolplugin.cpp \
           ktbrushmanager.cpp 
QT += xml gui
KDEV_QTVER = 4
INCLUDEPATH += ../../src/store \
../../src/dlib/dgui \
../../src/dlib/dcore \
../../3rdparty/potrace \
../../3rdparty/quazip \
../../src/dlib
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
TARGET = ktoon
CONFIG += release \
warn_on \
dll
TEMPLATE = lib
macx{
  LIBS += -ldcore -ldgui
  QMAKE_LIBDIR = ../../src/dlib/dcore ../../src/dlib/dgui
}
