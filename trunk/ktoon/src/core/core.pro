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
ktdata.path = $(KTOON_HOME)/data 
kttrans.files += *.qm 
kttrans.path = $(KTOON_HOME)/data/translations 
ktbin.files += ../../bin/ktoon 
ktbin.path = $(KTOON_HOME)/bin 
TRANSLATIONS += ktoon_es.ts \
                ktoon_fr.ts \
                ktoon_ru.ts \
                qt_es.ts 
HEADERS += ktoon.h \
           splash.h \
           ktfiledialog.h 
SOURCES += ktoon.cpp \
           main.cpp \
           splash.cpp \
           ktfiledialog.cpp 
TARGETDEPS += ../../src/dialogs/libdialogs.a \
../../src/qt/draw/libdraw.a \
../../src/qt/tools/libtools.a \
../../src/qt/cameras/libcameras.a \
../../src/opengl/tools/libtools.a \
../../src/opengl/cameras/libcameras.a \
../../src/opengl/draw/libdraw.a \
../../src/store/libstore.a \
../../src/library/liblibrary.a \
../../src/timeline/libtimeline.a \
../../src/lib/liblib.a \
../../src/images/images/libimages.a \
../../src/exposure/libexposure.a
LIBS += ../../src/dialogs/libdialogs.a \
../../src/images/images/libimages.a \
../../src/opengl/draw/libdraw.a \
../../src/opengl/tools/libtools.a \
../../src/opengl/cameras/libcameras.a \
../../src/qt/draw/libdraw.a \
../../src/qt/tools/libtools.a \
../../src/qt/cameras/libcameras.a \
../../src/store/libstore.a \
../../src/library/liblibrary.a \
../../src/timeline/libtimeline.a \
../../src/exposure/libexposure.a \
../../src/lib/liblib.a
INCLUDEPATH += ../../src/exposure \
../../src/dialogs \
../../src/qt/cameras \
../../src/qt/tools \
../../src/qt/draw \
../../src/opengl/draw \
../../src/opengl/tools \
../../src/opengl/cameras \
../../src/store \
../../src/library \
../../src/timeline \
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
