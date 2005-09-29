# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/modules/opengl/cameras
# Destiono es una biblioteca:  

INCLUDEPATH += ../../../../src/modules/opengl/draw \
               ../../../../src/modules/opengl/tools \
               ../../../../src/store \
               ../../../../src/lib 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
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
QT += qt3support xml opengl
