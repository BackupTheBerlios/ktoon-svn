# Archivo generado por el gestor qmake de kdevelop. 
# ------------------------------------------- 
# Subdirectorio relativo al directorio principal del proyecto: ./src/dialogs
# Destiono es una biblioteca:  

HEADERS += preferences.h \
           ktthemeselector.h \
           ktabout.h 
SOURCES += preferences.cpp \
           ktthemeselector.cpp \
           ktabout.cpp 
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
QT += qt3support xml opengl
