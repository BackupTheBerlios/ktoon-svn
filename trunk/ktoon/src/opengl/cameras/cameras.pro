# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/opengl/cameras
# Destiono es una biblioteca:  

TARGETDEPS += ../../../src/lib/liblib.a 
LIBS += ../../../src/lib/liblib.a 
INCLUDEPATH += ../../../src/dialogs \
               ../../../src/core \
               ../../../src/qt/cameras \
               ../../../src/qt/tools \
               ../../../src/qt/draw \
               ../../../src/opengl/draw \
               ../../../src/opengl/tools \
               ../../../src/opengl/cameras \
               ../../../src/store \
               ../../../src/library \
               ../../../src/exposure \
               ../../../src/timeline \
               ../../../src/lib \
               ../../../src/images/images \
               ../../../src/images/sequences \
               ../../../src/images/cursors \
               ../../../src/images/icons 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += glrendercamerapreview.h \
           glsidecameraview.h \
           gltopcameraview.h 
SOURCES += glrendercamerapreview.cpp \
           glsidecameraview.cpp \
           gltopcameraview.cpp 
