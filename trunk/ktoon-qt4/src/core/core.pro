# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/core
# Destiono es una aplicaci??n: ../../bin/ktoon

QT += qt3support xml opengl
INSTALLS += ktbin \
            kttrans \
            ktdata 
ktdata.files += credits.txt \
                ../../COPYING \
                ../../Changelog 
ktdata.path = /data 
kttrans.files += *.qm 
kttrans.path = /data/translations 
ktbin.files += ../../bin/ktoon 
ktbin.path = /bin 
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
              ../../src/modules/opengl/draw/libdraw.a \
              ../../src/modules/opengl/tools/libtools.a \
              ../../src/modules/opengl/cameras/libcameras.a \
              ../../src/modules/arthury/animation/libanimation.a \
              ../../src/modules/arthury/illustration/libillustration.a \
              ../../src/lib/libktoon.so 
LIBS += ../../src/dialogs/libdialogs.a \
        ../../src/components/brushes/libbrushes.a \
        ../../src/components/colorpalette/libcolorpalette.a \
        ../../src/components/scenes/libscenes.a \
        ../../src/components/ktoonscript/libktoonscript.a \
        ../../src/components/exposure/libexposure.a \
        ../../src/components/timeline/libtimeline.a \
        ../../src/components/library/liblibrary.a \
        ../../src/modules/opengl/draw/libdraw.a \
        ../../src/modules/opengl/tools/libtools.a \
        ../../src/modules/opengl/cameras/libcameras.a \
        ../../src/modules/arthury/animation/libanimation.a \
        ../../src/modules/arthury/illustration/libillustration.a \
        ../../src/dlslib/libdlslib.a \
        ../../src/store/libstore.a \
         \
         \
        -lktoon 
INCLUDEPATH += ../../src/lib \
               ../../src/modules/opengl/draw \
               ../../src/modules/opengl/tools \
               ../../src/modules/opengl/cameras \
               ../../src/modules/arthury/interfaces \
               ../../src/modules/arthury/animation \
               ../../src/modules/arthury/illustration \
               ../../src/components/ktoonscript \
               ../../src/components/exposure \
               ../../src/components/timeline \
               ../../src/components/library \
               ../../src/components/colorpalette \
               ../../src/components/scenes \
               ../../src/components/brushes \
               ../../src/dialogs \
               ../../src/store \
               ../../src/dlslib 
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
TRANSLATIONS += ktoon_es.ts \
                ktoon_fr.ts \
                ktoon_ru.ts \
                qt_es.ts 
HEADERS += ktoon.h \
           splash.h \
           ktdocumentruler.h \
           ktviewdocument.h \
           ktmainwindow.h \
           ktstatusbar.h \
           ktpaintareacontainer.h \
           ktnewproject.h \
           ktviewcamera.h \
           ktcamerabar.h 
SOURCES += ktoon.cpp \
           main.cpp \
           splash.cpp \
           ktdocumentruler.cpp \
           ktviewdocument.cpp \
           ktmainwindow.cpp \
           ktstatusbar.cpp \
           ktpaintareacontainer.cpp \
           ktnewproject.cpp \
           ktviewcamera.cpp \
           ktcamerabar.cpp 
