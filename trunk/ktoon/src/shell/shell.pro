# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/shell
# Target is an application:  ../../bin/ktoon.bin

INSTALLS += ktdata \
            target 
target.path = /bin/ 
ktdata.files += data/* 
ktdata.path = /data 
TRANSLATIONS += data/translations/ktoon_es.ts \
                data/translations/ktoon_fr.ts \
                data/translations/ktoon_ru.ts 
HEADERS += ktmainwindow.h \
           ktstatusbar.h \
           ktnewproject.h \
           ktsplash.h \
           crashhandler.h \
           crashwidget.h \
           ktapplication.h \
           configwizard.h \
           ktlocalprojectmanagerhandler.h 
SOURCES += main.cpp \
           ktmainwindow.cpp \
           ktstatusbar.cpp \
           ktnewproject.cpp \
           ktsplash.cpp \
           crashhandler.cpp \
           crashwidget.cpp \
           ktapplication.cpp \
           configwizard.cpp \
           ktmainwindow-gui.cpp \
           ktlocalprojectmanagerhandler.cpp 
include(shell_config.pri)
include(../../ktconfig.pri)
KDEV_QTVER = 4
TARGET = ../../bin/ktoon.bin
TEMPLATE = app
linux-g++{
  TARGETDEPS += ../../src/dlib/dcore/libdcore.so \
  ../../src/dlib/dgui/libdgui.so \
  ../../src/libktoon/libktoon.so \
  ../../src/libktui/libktui.a \
  ../../src/store/libstore.so \
  ../../src/components/pen/libpen.a \
  ../../src/components/kinas/libkinas.a \
  ../../src/components/help/libhelp.a \
  ../../src/components/import/libimport.a \
  ../../src/components/export/libexport.a \
  ../../src/components/exposure/libexposure.a \
  ../../src/components/timeline/libtimeline.a \
  ../../src/components/library/liblibrary.a \
  ../../src/components/colorpalette/libcolorpalette.a \
  ../../src/components/scenes/libscenes.a
}
