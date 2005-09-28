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
ktdata.path = /data 
kttrans.files += *.qm 
kttrans.path = /data/translations 
ktbin.files += ../../bin/ktoon 
ktbin.path = /bin 
TRANSLATIONS += ktoon_es.ts \
                ktoon_fr.ts \
                ktoon_ru.ts \
                qt_es.ts 
HEADERS += ktoon.h \
           splash.h \
           ktdocumentruler.h \
           ktviewdocument.h \
           ktmainwindow.h \
           ktstatusbar.h 
SOURCES += ktoon.cpp \
           main.cpp \
           splash.cpp \
           ktdocumentruler.cpp \
           ktviewdocument.cpp \
           ktmainwindow.cpp \
           ktstatusbar.cpp 
QT += qt3support opengl xml
TARGETDEPS += ../../src/dialogs/libdialogs.a \
../../src/qt/draw/libdraw.a \
../../src/qt/tools/libtools.a \
../../src/qt/cameras/libcameras.a \
../../src/opengl/tools/libtools.a \
../../src/opengl/cameras/libcameras.a \
../../src/opengl/draw/libdraw.a \
../../src/store/libstore.a \
../../src/lib/liblib.a \
../../src/images/images/libimages.a \
../../src/dlslib/libdlslib.a \
../../src/components/colorpalette/libcolorpalette.a \
../../src/components/scenes/libscenes.a \
../../src/components/brushes/libbrushes.a \
../../src/components/ktoonscript/libktoonscript.a \
../../src/components/exposure/libexposure.a \
../../src/components/timeline/libtimeline.a \
../../src/components/library/liblibrary.a
LIBS += ../../src/dialogs/libdialogs.a \
../../src/components/brushes/libbrushes.a \
../../src/components/colorpalette/libcolorpalette.a \
../../src/components/scenes/libscenes.a \
../../src/components/ktoonscript/libktoonscript.a \
../../src/components/exposure/libexposure.a \
../../src/components/timeline/libtimeline.a \
../../src/components/library/liblibrary.a \
../../src/opengl/draw/libdraw.a \
../../src/opengl/tools/libtools.a \
../../src/opengl/cameras/libcameras.a \
../../src/qt/draw/libdraw.a \
../../src/qt/tools/libtools.a \
../../src/qt/cameras/libcameras.a \
../../src/store/libstore.a \
../../src/dlslib/libdlslib.a \
../../src/lib/liblib.a \
../../src/images/images/libimages.a
INCLUDEPATH += ../../src/components/ktoonscript \
../../src/components/exposure \
../../src/components/timeline \
../../src/components/library \
../../src/components/colorpalette \
../../src/components/scenes \
../../src/components/brushes \
../../src/dialogs \
../../src/qt/cameras \
../../src/qt/tools \
../../src/qt/draw \
../../src/opengl/draw \
../../src/opengl/tools \
../../src/opengl/cameras \
../../src/store \
../../src/dlslib \
../../src/lib \
../../src/images/sequences \
../../src/images/icons \
../../src/images/cursors \
../../src/images/images
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
TARGET = ../../bin/ktoon
CONFIG += release \
warn_on \
qt \
opengl
TEMPLATE = app
