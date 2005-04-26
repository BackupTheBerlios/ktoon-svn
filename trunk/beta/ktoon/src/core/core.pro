# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/core
# Destiono es una aplicaci??n: ../../bin/ktoon

TARGETDEPS += ../../src/images/libimages.a \
              ../../src/tools/libtools.a \
              ../../src/timeline/libtimeline.a \
              ../../src/store/libstore.a \
              ../../src/scenes/libscenes.a \
              ../../src/properties/libproperties.a \
              ../../src/modal/libmodal.a \
              ../../src/library/liblibrary.a \
              ../../src/gl/libgl.a \
              ../../src/exposure/libexposure.a \
              ../../src/drawqt/libdrawqt.a \
              ../../src/color/libcolor.a \
              ../../src/cameras/libcameras.a \
              ../../src/brushes/libbrushes.a 
LIBS += ../../src/brushes/libbrushes.a \
        ../../src/gl/libgl.a \
        ../../src/images/libimages.a \
        ../../src/tools/libtools.a \
        ../../src/timeline/libtimeline.a \
        ../../src/store/libstore.a \
        ../../src/scenes/libscenes.a \
        ../../src/properties/libproperties.a \
        ../../src/modal/libmodal.a \
        ../../src/library/liblibrary.a \
        ../../src/exposure/libexposure.a \
        ../../src/drawqt/libdrawqt.a \
        ../../src/color/libcolor.a \
        ../../src/cameras/libcameras.a \
        ../../ming/libming.a 
INCLUDEPATH += ../../src/images \
               ../../src/tools \
               ../../src/timeline \
               ../../src/store \
               ../../src/scenes \
               ../../src/properties \
               ../../src/modal \
               ../../src/library \
               ../../src/gl \
               ../../src/exposure \
               ../../src/drawqt \
               ../../src/color \
               ../../src/cameras \
               ../../src/brushes 
TARGET = ../../bin/ktoon 
CONFIG += release \
          warn_on \
          opengl \
          thread \
          x11 
TEMPLATE = app 
HEADERS += ktoon.h 
SOURCES += ktoon.cpp \
           main.cpp 
