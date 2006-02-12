# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/core
# Target is an application:  ../../bin/ktoon

INSTALLS += kttrans \
            ktdata \
            target 
target.path = /bin/ 
ktdata.files += data/* 
ktdata.path = /data 
kttrans.files += *.qm 
kttrans.path = /data/translations 
TRANSLATIONS += ktoon_es.ts \
                ktoon_fr.ts \
                ktoon_ru.ts \
                qt_es.ts 
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
           ktconfigurationarea.h 
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
           ktconfigurationarea.cpp 
QT += xml opengl
KDEV_QTVER = 4
TARGETDEPS += ../../src/dlslib/libdlslib.a \
../../src/dialogs/libdialogs.a \
../../src/components/colorpalette/libcolorpalette.a \
../../src/components/scenes/libscenes.a \
../../src/components/brushes/libbrushes.a \
../../src/components/exposure/libexposure.a \
../../src/components/timeline/libtimeline.a \
../../src/components/library/liblibrary.a \
../../src/modules/arthury/animation/libanimation.a \
../../src/modules/arthury/illustration/libillustration.a \
../../src/lib/libktoon.so \
../../src/components/gceditor/libgceditor.a \
../../src/components/import/libimport.a \
../../src/components/help/libhelp.a \
../../src/store/libstore.so \
../../src/ktoonstyle/libktoonstyle.a \
../../src/components/export/ui/libui.a \
../../src/components/kinas/libkinas.a \
../../3rdparty/quazip/libquazip.a
LIBS += ../../src/ktoonstyle/libktoonstyle.a \
../../src/dialogs/libdialogs.a \
../../src/components/brushes/libbrushes.a \
../../src/components/colorpalette/libcolorpalette.a \
../../src/components/scenes/libscenes.a \
../../src/components/exposure/libexposure.a \
../../src/components/timeline/libtimeline.a \
../../src/components/library/liblibrary.a \
../../src/modules/arthury/animation/libanimation.a \
../../src/modules/arthury/illustration/libillustration.a \
../../src/dlslib/libdlslib.a \
-lktoon \
../../src/components/gceditor/libgceditor.a \
../../src/components/import/libimport.a \
../../src/components/help/libhelp.a \
-lstore \
../../src/components/export/ui/libui.a \
../../src/components/kinas/libkinas.a \
../../3rdparty/quazip/libquazip.a
INCLUDEPATH += ../../src/ktoonstyle \
../../src/modules/arthury/interfaces \
../../src/modules/arthury/animation \
../../src/modules/arthury/illustration \
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
../../src/components/brushes \
../../src/dialogs \
../../src/store \
../../src/dlslib \
../../src/lib \
../../src/interfaces \
../../3rdparty/quazip
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
QMAKE_LIBDIR = ../../src/lib \
../../src/store
TARGET = ../../bin/ktoon
CONFIG += release \
warn_on
TEMPLATE = app
