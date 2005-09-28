# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/dialogs
# Destiono es una biblioteca:  

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
INCLUDEPATH += ../../src/core \
../../src/exposure \
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
../../src/images/images
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
CONFIG += release \
warn_on \
staticlib
TEMPLATE = lib
QT += xml opengl qt3support
