# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/core
# Target is an application:  ../../bin/ktoon

QT += xml svg opengl 
KDEV_QTVER = 4 
INSTALLS += ktdata \
            target 
target.path = /bin/ 
ktdata.files += data/* 
ktdata.path = /data 
LIBS += -ldcore \
        -ldgui \
        -lktoon \
        ../../src/ktoonstyle/libktoonstyle.a \
        ../../src/dialogs/libdialogs.a \
        ../../src/components/colorpalette/libcolorpalette.a \
        ../../src/components/scenes/libscenes.a \
        ../../src/components/exposure/libexposure.a \
        ../../src/components/timeline/libtimeline.a \
        ../../src/components/library/liblibrary.a \
        ../../src/modules/arthury/animation/libanimation.a \
        ../../src/modules/arthury/illustration/libillustration.a \
        ../../src/components/gceditor/libgceditor.a \
        ../../src/components/import/libimport.a \
        ../../src/components/help/libhelp.a \
        -lstore \
        ../../src/components/export/ui/libui.a \
        ../../src/components/kinas/libkinas.a \
        ../../3rdparty/quazip/libquazip.a \
        ../../src/components/pen/libpen.a 
INCLUDEPATH += ../../src/ktoonstyle \
               ../../src/modules/arthury/interfaces \
               ../../src/modules/arthury/animation \
               ../../src/modules/arthury/illustration \
               ../../src/components/pen \
               ../../src/components/kinas \
               ../../src/components/help \
               ../../src/components/gceditor \
               ../../src/components/import \
               ../../src/components/export/ui \
               ../../src/components/exposure \
               ../../src/components/timeline \
               ../../src/components/library \
               ../../src/components/colorpalette \
               ../../src/components/scenes \
               ../../src/dialogs \
               ../../src/store \
               ../../src/ktoonlib \
               ../../src/dlib/dgui \
               ../../src/dlib/dcore \
               ../../3rdparty/quazip 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
QMAKE_LIBDIR = ../../src/dlib/dcore \
               ../../src/dlib/dgui \
               ../../src/ktoonlib \
               ../../src/store 
TARGET = ../../bin/ktoon 
CONFIG += release \
          warn_on 
TEMPLATE = app 
TRANSLATIONS += data/translations/ktoon_es.ts \
                data/translations/ktoon_fr.ts \
                data/translations/ktoon_ru.ts 
HEADERS += ktdocumentruler.h \
           ktviewdocument.h \
           ktmainwindow.h \
           ktstatusbar.h \
           ktpaintareacontainer.h \
           ktnewproject.h \
           ktviewcamera.h \
           ktcamerabar.h \
           ktosd.h \
           ktsplash.h \
           crashhandler.h \
           crashwidget.h \
           ktworkspace.h \
           ktpackagehandler.h \
           ktconfigurationarea.h \
           ktapplication.h \
           configwizard.h \
           ktdrawingareaproperties.h \
           ktpluginmanager.h 
SOURCES += main.cpp \
           ktdocumentruler.cpp \
           ktviewdocument.cpp \
           ktmainwindow.cpp \
           ktstatusbar.cpp \
           ktpaintareacontainer.cpp \
           ktnewproject.cpp \
           ktviewcamera.cpp \
           ktcamerabar.cpp \
           ktmainwindow-gui.cpp \
           ktmainwindow-project.cpp \
           ktosd.cpp \
           ktsplash.cpp \
           crashhandler.cpp \
           crashwidget.cpp \
           ktworkspace.cpp \
           ktpackagehandler.cpp \
           ktconfigurationarea.cpp \
           ktapplication.cpp \
           configwizard.cpp \
           ktdrawingareaproperties.cpp \
           ktpluginmanager.cpp 
linux-g++{
  TARGETDEPS += ../../src/dlib/dcore/libdcore.so \
  ../../src/dlib/dgui/libdgui.so \
  ../../src/ktoonlib/libktoon.so \
  ../../src/dialogs/libdialogs.a \
  ../../src/store/libstore.so \
  ../../src/modules/arthury/animation/libanimation.a \
  ../../src/modules/arthury/illustration/libillustration.a \
  ../../src/components/pen/libpen.a \
  ../../src/components/kinas/libkinas.a \
  ../../src/components/help/libhelp.a \
  ../../src/components/gceditor/libgceditor.a \
  ../../src/components/import/libimport.a \
  ../../src/components/export/ui/libui.a \
  ../../src/components/exposure/libexposure.a \
  ../../src/components/timeline/libtimeline.a \
  ../../src/components/library/liblibrary.a \
  ../../src/components/colorpalette/libcolorpalette.a \
  ../../src/components/scenes/libscenes.a
}