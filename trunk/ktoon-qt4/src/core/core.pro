# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/core
# Destiono es una aplicaci??n: ../../bin/ktoon

INSTALLS += ktbin \
            kttrans \
            ktdata 
ktdata.files += credits.txt \
                ../../COPYING \
                ../../Changelog 
ktbin.files += ../../bin/ktoon 
ktbin.path = /bin 
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
           ktsplash.h 
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
           ktsplash.cpp 
QT += qt3support xml
TARGETDEPS += ../../src/dlslib/libdlslib.a \
              ../../src/store/libstore.a \
              ../../src/dialogs/libdialogs.a \
              ../../src/components/colorpalette/libcolorpalette.a \
              ../../src/components/scenes/libscenes.a \
              ../../src/components/brushes/libbrushes.a \
              ../../src/components/ktoonscript/libktoonscript.a \
              ../../src/components/exposure/libexposure.a \
              ../../src/components/timeline/libtimeline.a \
              ../../src/components/library/liblibrary.a \
              ../../src/modules/arthury/animation/libanimation.a \
              ../../src/modules/arthury/illustration/libillustration.a \
              ../../src/lib/libktoon.so \
              ../../src/components/gceditor/libgceditor.a \
              ../../src/components/import/libimport.a \
              ../../src/components/export/libexport.a \
              ../../src/components/help/libhelp.a 
LIBS += ../../src/dialogs/libdialogs.a \
        ../../src/components/brushes/libbrushes.a \
        ../../src/components/colorpalette/libcolorpalette.a \
        ../../src/components/scenes/libscenes.a \
        ../../src/components/ktoonscript/libktoonscript.a \
        ../../src/components/exposure/libexposure.a \
        ../../src/components/timeline/libtimeline.a \
        ../../src/components/library/liblibrary.a \
        ../../src/modules/arthury/animation/libanimation.a \
        ../../src/modules/arthury/illustration/libillustration.a \
        ../../src/dlslib/libdlslib.a \
        ../../src/store/libstore.a \
        -lktoon \
        ../../src/components/gceditor/libgceditor.a \
        ../../src/components/import/libimport.a \
        ../../src/components/export/libexport.a \
        ../../src/components/help/libhelp.a 
INCLUDEPATH += ../../src/modules/arthury/interfaces \
               ../../src/modules/arthury/animation \
               ../../src/modules/arthury/illustration \
               ../../src/components/help \
               ../../src/components/gceditor \
               ../../src/components/import \
               ../../src/components/export \
               ../../src/components/ktoonscript \
               ../../src/components/exposure \
               ../../src/components/timeline \
               ../../src/components/library \
               ../../src/components/colorpalette \
               ../../src/components/scenes \
               ../../src/components/brushes \
               ../../src/dialogs \
               ../../src/store \
               ../../src/dlslib \
               ../../src/lib 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
QMAKE_LIBDIR = ../../src/lib 
TARGET = ../../bin/ktoon 
CONFIG += release \
          warn_on \
          qt \
          opengl 
TEMPLATE = app 
