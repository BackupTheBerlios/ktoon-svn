# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/core
# Destiono es una aplicaci??n: ../../bin/ktoon

INSTALLS += ktbin \
            kttrans 
kttrans.files += *.qm 
kttrans.path = $(KTOON_HOME)/data/translations 
ktbin.files += ../../bin/ktoon 
ktbin.path = $(KTOON_HOME)/bin 
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
              ../../src/images/images/libimages.a 
LIBS += ../../src/images/images/libimages.a \
        ../../src/opengl/draw/libdraw.a \
        ../../src/lib/liblib.a \
        ../../src/opengl/tools/libtools.a \
        ../../src/opengl/cameras/libcameras.a \
        ../../src/qt/draw/libdraw.a \
        ../../src/qt/tools/libtools.a \
        ../../src/qt/cameras/libcameras.a \
        ../../src/store/libstore.a \
        ../../src/library/liblibrary.a \
        ../../src/dialogs/libdialogs.a \
        ../../src/timeline/libtimeline.a 
INCLUDEPATH += ../../src/dialogs \
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
DEFINES = KTHOME=\"$(KTOON_HOME)\" 
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
           about.h \
           splash.h \
           export.h \
           import.h \
           preferences.h \
           properties.h \
           ktconfigdocument.h 
SOURCES += ktoon.cpp \
           main.cpp \
           about.cpp \
           splash.cpp \
           export.cpp \
           import.cpp \
           preferences.cpp \
           properties.cpp \
           ktconfigdocument.cpp 
