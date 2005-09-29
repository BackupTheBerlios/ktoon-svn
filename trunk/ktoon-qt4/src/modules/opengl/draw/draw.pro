# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/modules/opengl/draw
# Destiono es una biblioteca:  

INCLUDEPATH += ../../../../src/modules/opengl/tools \
               ../../../../src/modules/opengl/cameras \
               ../../../../src/dialogs \
               ../../../../src/store \
               ../../../../src/lib \
               ../../../../src/images/images 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += drawingarea.h \
           glcontrol.h \
           glgrid.h 
SOURCES += drawingarea.cpp \
           glcontrol.cpp \
           glgrid.cpp 
QT += qt3support xml opengl
