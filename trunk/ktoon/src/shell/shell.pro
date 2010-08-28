# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/shell
# Target is an application:  ../../bin/ktoon.bin

INSTALLS += ktdata \
            launcher \
            desktop \
            icons \
            target 
target.path = /bin/ 
launcher.files += ../../launcher/ktoon
launcher.path = /bin/
desktop.files += ../../launcher/ktoon.desktop
desktop.path = /applications/
icons.files += ../../launcher/icons/ktoon.png
icons.path = /pixmaps/
ktdata.files += data/* 
ktdata.path = /data 
TRANSLATIONS += data/translations/ktoon_es.ts \
                data/translations/ktoon_ca.ts \
                data/translations/ktoon_ru.ts
HEADERS += ktmainwindow.h \
           ktstatusbar.h \
           ktnewproject.h \
           ktsplash.h \
           ktcrashhandler.h \
           ktcrashwidget.h \
           ktapplication.h \
           configwizard.h \
           ktlocalprojectmanagerhandler.h
SOURCES += main.cpp \
           ktmainwindow.cpp \
           ktstatusbar.cpp \
           ktnewproject.cpp \
           ktsplash.cpp \
           ktcrashhandler.cpp \
           ktcrashwidget.cpp \
           ktapplication.cpp \
           configwizard.cpp \
           ktmainwindow_gui.cpp \
           ktlocalprojectmanagerhandler.cpp

include(shell_config.pri)
include(../../ktconfig.pri)
TARGET = ../../bin/ktoon.bin
TEMPLATE = app
linux-g{
    TARGETDEPS += ../../src/libktoon/libktoon.so \
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
  ../../src/components/scenes/libscenes.a \
  ../../src/components/twitter/libtwitter.a
}

