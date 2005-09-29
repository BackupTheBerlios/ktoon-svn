# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/dialogs
# Destiono es una biblioteca:  

INCLUDEPATH += ../../src/modules/opengl/draw \
               ../../src/modules/opengl/tools \
               ../../src/modules/opengl/cameras \
               ../../src/core \
               ../../src/store \
               ../../src/lib \
               ../../src/images/images 
MOC_DIR = .moc 
UI_DIR = .ui 
OBJECTS_DIR = .obj 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += about.h \
           export.h \
           import.h \
           preferences.h \
           properties.h \
           ktthemeselector.h \
           ktcamerapreview.h \
           tools.h 
SOURCES += about.cpp \
           export.cpp \
           import.cpp \
           preferences.cpp \
           properties.cpp \
           ktthemeselector.cpp \
           ktcamerapreview.cpp \
           tools.cpp 
QT += qt3support xml opengl
